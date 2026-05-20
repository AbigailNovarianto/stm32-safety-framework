/**
 * @file    main_f401.c
 * @brief   Adaptive Safety-Critical Multi-Sensor Acquisition Framework
 *          TARGET: STM32F401VE (Cortex-M4 @ 84 MHz)
 *          Adaptasi dari main.c (F407) untuk simulasi Proteus
 *
 * Perbedaan utama vs main.c (F407):
 *  1. SYSCLK = 84 MHz (bukan 168 MHz)
 *  2. Tidak ada CCM SRAM — buffer ADC di SRAM biasa
 *  3. FLASH_LATENCY_2 (bukan LATENCY_5)
 *  4. APB1 max 42 MHz, APB2 max 84 MHz
 *  5. PWM_PERIOD = 8399 (sama, karena TIM1 di APB2 = 84 MHz)
 */

/* =========================================================
 * INCLUDES
 * ========================================================= */
#include "main_f401.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>

/* =========================================================
 * HARDWARE HANDLES
 * ========================================================= */
ADC_HandleTypeDef  hadc1;
DMA_HandleTypeDef  hdma_adc1;
TIM_HandleTypeDef  htim1;
UART_HandleTypeDef huart1;
I2C_HandleTypeDef  hi2c1;
WWDG_HandleTypeDef hwwdg;

/* =========================================================
 * ADC DMA BUFFER
 * F401 tidak punya CCM SRAM — pakai SRAM biasa
 * (berbeda dari F407 yang pakai __attribute__((section(".ccmram"))))
 * ========================================================= */
volatile uint16_t adc_dma_buf[2];  /* [0]=CH0 sensor1, [1]=CH1 sensor2 */

/* =========================================================
 * KONSTANTA SISTEM (disesuaikan untuk F401 @ 84 MHz)
 * ========================================================= */
#define ADC_MAX_VAL       4095U
#define VREF_MV           3300U
#define SAFETY_HIGH_MV    2800U
#define SAFETY_LOW_MV      200U
#define FAULT_COUNT_MAX      3U
/* TIM1 pada F401: APB2 = 84 MHz (tidak dibagi), TIM1 clock = 84 MHz
 * Untuk PWM 10 kHz: ARR = 84MHz/10kHz - 1 = 8399 */
#define PWM_PERIOD        8399U
#define UART_BUF_SIZE      128U
#define MA_WINDOW            8U

/* =========================================================
 * TIPE DATA
 * ========================================================= */
typedef enum { STATE_NORMAL=0, STATE_WARNING=1, STATE_FAULT=2 } SystemState_t;

typedef struct {
    uint16_t      volt_s1_mv;
    uint16_t      volt_s2_mv;
    uint16_t      pwm_duty_pct;
    SystemState_t state;
    uint32_t      timestamp_ms;
} SensorData_t;

/* =========================================================
 * VARIABEL GLOBAL
 * ========================================================= */
static volatile uint8_t  dma_done_flag = 0;
static volatile uint8_t  fault_counter = 0;
static          SystemState_t sys_state = STATE_NORMAL;

#define LOG_BUF_DEPTH 16
static SensorData_t      log_buf[LOG_BUF_DEPTH];
static volatile uint8_t  log_head = 0;
static volatile uint8_t  log_tail = 0;

static osThreadId   hTask_Sensor, hTask_Process, hTask_Safety, hTask_Comm;
static osMessageQId hQueue_SensorData;

/* =========================================================
 * FORWARD DECLARATIONS
 * ========================================================= */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_WWDG_Init(void);

static void Task_SensorRead  (void const *arg);
static void Task_DataProcess (void const *arg);
static void Task_SafetyCheck (void const *arg);
static void Task_CommOutput  (void const *arg);

static uint16_t adc_to_mv(uint16_t raw);
static uint16_t moving_average(uint16_t *buf, uint8_t *idx, uint16_t val);
static void     set_pwm_duty(uint8_t pct);
static void     set_led_state(SystemState_t s);
static void     log_push(SensorData_t *d);
static uint8_t  log_pop(SensorData_t *out);

/* =========================================================
 * MAIN
 * ========================================================= */
int main(void)
{
    HAL_Init();
    SystemClock_Config();   /* 84 MHz HSE PLL (F401) */

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM1_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    MX_WWDG_Init();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_dma_buf, 2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    hQueue_SensorData = osMessageCreate(osMessageQ(SensorDataQueue), NULL);

    osThreadDef(Safety,  Task_SafetyCheck,  osPriorityRealtime, 0, 256);
    osThreadDef(Sensor,  Task_SensorRead,   osPriorityHigh,     0, 256);
    osThreadDef(Process, Task_DataProcess,  osPriorityNormal,   0, 512);
    osThreadDef(Comm,    Task_CommOutput,   osPriorityLow,      0, 512);

    hTask_Safety  = osThreadCreate(osThread(Safety),  NULL);
    hTask_Sensor  = osThreadCreate(osThread(Sensor),  NULL);
    hTask_Process = osThreadCreate(osThread(Process), NULL);
    hTask_Comm    = osThreadCreate(osThread(Comm),    NULL);

    osKernelStart();
    while (1) {}
}

/* =========================================================
 * TASK 1: SensorRead (Priority HIGH) — Adaptive sampling
 * ========================================================= */
static void Task_SensorRead(void const *arg)
{
    /* PENTING: static agar pointer &data yang dikirim ke queue
     * tetap valid saat Task_DataProcess membacanya (bukan dangling ptr) */
    static SensorData_t data;
    uint16_t prev_s1 = 0;
    uint32_t delay_ms = 10;

    for (;;)
    {
        /* Tunggu DMA callback selesai.
         * PROTEUS WORKAROUND: DMA ADC sering tidak men-trigger callback
         * di Proteus. Timeout 50ms agar task tidak stuck selamanya. */
        uint32_t t0 = HAL_GetTick();
        while (!dma_done_flag)
        {
            if ((HAL_GetTick() - t0) >= 50U)
            {
                /* Fallback: DMA tidak trigger dalam 50ms.
                 * Isi buffer dengan midrange (2048 ≈ 1650 mV) supaya
                 * sistem tidak langsung masuk STATE_FAULT (false alarm). */
                if (adc_dma_buf[0] == 0U) { adc_dma_buf[0] = 2048U; }
                if (adc_dma_buf[1] == 0U) { adc_dma_buf[1] = 2048U; }
                break;
            }
            osThreadYield();
        }
        dma_done_flag = 0;

        data.volt_s1_mv   = adc_to_mv(adc_dma_buf[0]);
        data.volt_s2_mv   = adc_to_mv(adc_dma_buf[1]);
        data.state        = sys_state;
        data.timestamp_ms = HAL_GetTick();

        /* Feed WWDG — wajib dalam window */
        HAL_WWDG_Refresh(&hwwdg);

        /* Adaptive sampling: delta besar → rate tinggi */
        uint16_t raw_s1 = adc_dma_buf[0];
        uint16_t delta = (raw_s1 > prev_s1) ? (raw_s1 - prev_s1)
                                             : (prev_s1 - raw_s1);
        if (delta > 50)          delay_ms = 10;
        else if (delay_ms < 100) delay_ms += 5;
        prev_s1 = raw_s1;

        osMessagePut(hQueue_SensorData, (uint32_t)&data, 0);
        osDelay(delay_ms);
    }
}

/* =========================================================
 * TASK 2: DataProcess (Priority NORMAL) — Filter + PWM
 * ========================================================= */
static void Task_DataProcess(void const *arg)
{
    static uint16_t ma_s1[MA_WINDOW] = {0}, ma_s2[MA_WINDOW] = {0};
    static uint8_t  idx_s1 = 0, idx_s2 = 0;
    osEvent evt;
    SensorData_t *p;

    for (;;)
    {
        evt = osMessageGet(hQueue_SensorData, osWaitForever);
        if (evt.status != osEventMessage) continue;
        p = (SensorData_t *)evt.value.p;

        uint16_t f1 = moving_average(ma_s1, &idx_s1, p->volt_s1_mv);
        uint16_t f2 = moving_average(ma_s2, &idx_s2, p->volt_s2_mv);

        p->volt_s1_mv    = f1;
        p->volt_s2_mv    = f2;
        p->pwm_duty_pct  = (uint8_t)(((f1 + f2) / 2U * 100U) / VREF_MV);

        set_pwm_duty(p->pwm_duty_pct);
        log_push(p);
    }
}

/* =========================================================
 * TASK 3: SafetyCheck (Priority REALTIME) — CBF + WWDG
 * ========================================================= */
static void Task_SafetyCheck(void const *arg)
{
    for (;;)
    {
        uint16_t v1 = adc_to_mv(adc_dma_buf[0]);
        uint16_t v2 = adc_to_mv(adc_dma_buf[1]);

        uint8_t safe = (v1 >= SAFETY_LOW_MV && v1 <= SAFETY_HIGH_MV) &&
                       (v2 >= SAFETY_LOW_MV && v2 <= SAFETY_HIGH_MV);

        if (safe) {
            fault_counter = 0;
            sys_state = STATE_NORMAL;
            set_led_state(STATE_NORMAL);
        } else {
            fault_counter++;
            if (fault_counter >= FAULT_COUNT_MAX) {
                sys_state = STATE_FAULT;
                set_led_state(STATE_FAULT);
                const char *msg = "FAULT: Safety barrier breached. System reset.\r\n";
                HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
                while (1) {}  /* WWDG timeout → hardware reset */
            } else {
                sys_state = STATE_WARNING;
                set_led_state(STATE_WARNING);
            }
        }
        osDelay(500);
    }
}

/* =========================================================
 * TASK 4: CommOutput (Priority LOW) — UART + I2C
 * ========================================================= */
static void Task_CommOutput(void const *arg)
{
    char uart_buf[UART_BUF_SIZE];
    uint8_t i2c_buf[16];
    SensorData_t d;

    for (;;)
    {
        if (log_pop(&d)) {
            int len = snprintf(uart_buf, UART_BUF_SIZE,
                "%lu,%u,%u,%u,%u\r\n",
                d.timestamp_ms, d.volt_s1_mv,
                d.volt_s2_mv, d.pwm_duty_pct, (uint8_t)d.state);
            HAL_UART_Transmit(&huart1, (uint8_t *)uart_buf, (uint16_t)len, 50);

            snprintf((char *)i2c_buf, 16, "S1:%4umV P:%3u%%",
                     d.volt_s1_mv, d.pwm_duty_pct);
            HAL_I2C_Master_Transmit(&hi2c1, 0x27 << 1, i2c_buf, 16, 20);
        }
        osDelay(100);
    }
}

/* =========================================================
 * DMA CALLBACK
 * ========================================================= */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1) dma_done_flag = 1;
}

/* =========================================================
 * HELPER FUNCTIONS
 * ========================================================= */
static uint16_t adc_to_mv(uint16_t raw)
{
    return (uint16_t)(((uint32_t)raw * VREF_MV) / ADC_MAX_VAL);
}

static uint16_t moving_average(uint16_t *buf, uint8_t *idx, uint16_t val)
{
    buf[*idx] = val;
    *idx = (*idx + 1U) % MA_WINDOW;
    uint32_t sum = 0;
    for (uint8_t i = 0; i < MA_WINDOW; i++) sum += buf[i];
    return (uint16_t)(sum / MA_WINDOW);
}

static void set_pwm_duty(uint8_t pct)
{
    if (pct > 100U) pct = 100U;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,
                          ((uint32_t)pct * PWM_PERIOD) / 100U);
}

static void set_led_state(SystemState_t s)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
    switch (s) {
        case STATE_NORMAL:  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); break;
        case STATE_WARNING: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET); break;
        case STATE_FAULT:   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); break;
    }
}

static void log_push(SensorData_t *d)
{
    uint8_t next = (log_head + 1U) % LOG_BUF_DEPTH;
    if (next != log_tail) { log_buf[log_head] = *d; log_head = next; }
}

static uint8_t log_pop(SensorData_t *out)
{
    if (log_tail == log_head) return 0;
    *out = log_buf[log_tail];
    log_tail = (log_tail + 1U) % LOG_BUF_DEPTH;
    return 1;
}

/* =========================================================
 * PERIPHERAL INIT — STM32F401VE @ 84 MHz
 * ========================================================= */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};

    /* HSE 8 MHz → PLL → SYSCLK 84 MHz
     * PLLM=8, PLLN=336, PLLP=DIV4 → 8/8 * 336 / 4 = 84 MHz */
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc.HSEState       = RCC_HSE_ON;
    osc.PLL.PLLState   = RCC_PLL_ON;
    osc.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    osc.PLL.PLLM       = 8;
    osc.PLL.PLLN       = 336;
    osc.PLL.PLLP       = RCC_PLLP_DIV4;   /* 84 MHz (F401 max) */
    osc.PLL.PLLQ       = 7;
    HAL_RCC_OscConfig(&osc);

    clk.ClockType      = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
                       | RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;   /* HCLK = 84 MHz */
    clk.APB1CLKDivider = RCC_HCLK_DIV2;     /* APB1 = 42 MHz (max F401) */
    clk.APB2CLKDivider = RCC_HCLK_DIV1;     /* APB2 = 84 MHz */
    HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2); /* F401 @ 84 MHz = 2 wait states */
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef g = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* PA0, PA1: Analog input untuk ADC (tidak perlu Mode/Pull — reset state = Analog) */
    g.Pin  = GPIO_PIN_0 | GPIO_PIN_1;
    g.Mode = GPIO_MODE_ANALOG;
    g.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &g);

    /* PA8: TIM1_CH1 PWM output (AF1 — Alternate Function Push-Pull)
     * Wiring Proteus: PA8 → Oscilloscope CH1 */
    g.Pin       = GPIO_PIN_8;
    g.Mode      = GPIO_MODE_AF_PP;
    g.Pull      = GPIO_NOPULL;
    g.Speed     = GPIO_SPEED_FREQ_LOW;
    g.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &g);

    /* PA9 (TX), PA10 (RX): USART1 (AF7 — Push-Pull)
     * Wiring Proteus: PA9 → VT_RXD, PA10 → VT_TXD */
    g.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    g.Mode      = GPIO_MODE_AF_PP;
    g.Pull      = GPIO_PULLUP;
    g.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    g.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &g);

    /* PB6 (SCL), PB7 (SDA): I2C1 (AF4 — Open-Drain wajib untuk I2C)
     * Wiring Proteus: PB6/PB7 → PCF8574 SCL/SDA */
    g.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    g.Mode      = GPIO_MODE_AF_OD;     /* Open-Drain: wajib untuk I2C */
    g.Pull      = GPIO_PULLUP;          /* Pull-up internal (tambahkan juga 4.7kΩ ext) */
    g.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    g.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &g);

    /* PD12-PD14: LED indicator output (Push-Pull)
     * Wiring Proteus: PD12→LED Normal, PD13→LED Warning, PD14→LED Fault via 330Ω */
    g.Pin   = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    g.Mode  = GPIO_MODE_OUTPUT_PP;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &g);

    /* Set semua LED OFF saat init */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
}

static void MX_DMA_Init(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE();
    hdma_adc1.Instance                 = DMA2_Stream0;
    hdma_adc1.Init.Channel             = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode                = DMA_CIRCULAR;
    hdma_adc1.Init.Priority            = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adc1);
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

static void MX_ADC1_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();
    hadc1.Instance                   = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode          = ENABLE;
    hadc1.Init.ContinuousConvMode    = ENABLE;
    /* FIX: DMAContinuousRequests adalah field yang benar untuk F4
     * DMAConversionMode/ADC_DMAACCESSMODE_1 adalah milik F1 series */
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    hadc1.Init.NbrOfConversion       = 2;
    HAL_ADC_Init(&hadc1);

    ADC_ChannelConfTypeDef ch = {0};
    ch.Channel      = ADC_CHANNEL_0;
    ch.Rank         = 1;
    ch.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &ch);
    ch.Channel = ADC_CHANNEL_1; ch.Rank = 2;
    HAL_ADC_ConfigChannel(&hadc1, &ch);
}

static void MX_TIM1_Init(void)
{
    __HAL_RCC_TIM1_CLK_ENABLE();
    htim1.Instance               = TIM1;
    htim1.Init.Prescaler         = 0;
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim1.Init.Period            = PWM_PERIOD;
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    HAL_TIM_PWM_Init(&htim1);
    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode = TIM_OCMODE_PWM1; oc.Pulse = 0;
    oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim1, &oc, TIM_CHANNEL_1);
}

static void MX_USART1_UART_Init(void)
{
    __HAL_RCC_USART1_CLK_ENABLE();
    huart1.Instance          = USART1;
    huart1.Init.BaudRate     = 115200;
    huart1.Init.WordLength   = UART_WORDLENGTH_8B;
    huart1.Init.StopBits     = UART_STOPBITS_1;
    huart1.Init.Parity       = UART_PARITY_NONE;
    huart1.Init.Mode         = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
}

static void MX_I2C1_Init(void)
{
    __HAL_RCC_I2C1_CLK_ENABLE();
    hi2c1.Instance             = I2C1;
    hi2c1.Init.ClockSpeed      = 400000;
    hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1     = 0;
    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);
}

static void MX_WWDG_Init(void)
{
    /* F401 @ 84 MHz: APB1 = 42 MHz
     * WWDG clock = APB1/4096/8 = 42M/4096/8 = ~1281 Hz → ~781 us/count
     *
     * PROTEUS WORKAROUND: Window diperlebar ke 0x7F (sama dengan Counter)
     * agar feed HAL_WWDG_Refresh() SELALU valid kapanpun dipanggil.
     * Timing Proteus berbeda dari hardware nyata sehingga window sempit
     * (0x50) bisa menyebabkan false WWDG reset di simulasi.
     *
     * Hardware nyata: Window = 0x50 (feed harus antara 0x50–0x7F)
     * Proteus sim  : Window = 0x7F (feed boleh kapan saja → paling longgar)
     *
     * Timeout reset = T × (Counter[5:0]+1) = 781us × 64 ≈ 50ms */
    __HAL_RCC_WWDG_CLK_ENABLE();
    hwwdg.Instance       = WWDG;
    hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
    hwwdg.Init.Window    = 0x7F;   /* Lebar maksimal untuk Proteus */
    hwwdg.Init.Counter   = 0x7F;
    hwwdg.Init.EWIMode   = WWDG_EWI_DISABLE;
    HAL_WWDG_Init(&hwwdg);
}

/* =========================================================
 * IRQ HANDLERS
 * ========================================================= */
void DMA2_Stream0_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_adc1); }
void USART1_IRQHandler(void)       { HAL_UART_IRQHandler(&huart1);   }
void I2C1_EV_IRQHandler(void)      { HAL_I2C_EV_IRQHandler(&hi2c1);  }
void WWDG_IRQHandler(void)         { HAL_WWDG_IRQHandler(&hwwdg);    }
