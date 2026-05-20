/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "main_f401.h"
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
ADC_HandleTypeDef  hadc1;
DMA_HandleTypeDef  hdma_adc1;
TIM_HandleTypeDef  htim1;
UART_HandleTypeDef huart1;
I2C_HandleTypeDef  hi2c1;
WWDG_HandleTypeDef hwwdg;

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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

WWDG_HandleTypeDef hwwdg;

osThreadId SafetyTaskHandle;
osThreadId SensorTaskHandle;
osThreadId ProcessTaskHandle;
osThreadId CommTaskHandle;
/* USER CODE BEGIN PV */
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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_WWDG_Init(void);
void Task_SafetyCheck(void const * argument);
void Task_SensorRead(void const * argument);
void Task_DataProcess(void const * argument);
void Task_CommOutput(void const * argument);

/* USER CODE BEGIN PFP */
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
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_WWDG_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of SafetyTask */
  osThreadDef(SafetyTask, Task_SafetyCheck, osPriorityRealtime, 0, 256);
  SafetyTaskHandle = osThreadCreate(osThread(SafetyTask), NULL);

  /* definition and creation of SensorTask */
  osThreadDef(SensorTask, Task_SensorRead, osPriorityHigh, 0, 256);
  SensorTaskHandle = osThreadCreate(osThread(SensorTask), NULL);

  /* definition and creation of ProcessTask */
  osThreadDef(ProcessTask, Task_DataProcess, osPriorityNormal, 0, 512);
  ProcessTaskHandle = osThreadCreate(osThread(ProcessTask), NULL);

  /* definition and creation of CommTask */
  osThreadDef(CommTask, Task_CommOutput, osPriorityLow, 0, 512);
  CommTaskHandle = osThreadCreate(osThread(CommTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
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
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 8399;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief WWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_WWDG_Init(void)
{

  /* USER CODE BEGIN WWDG_Init 0 */

  /* USER CODE END WWDG_Init 0 */

  /* USER CODE BEGIN WWDG_Init 1 */

  /* USER CODE END WWDG_Init 1 */
  hwwdg.Instance = WWDG;
  hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
  hwwdg.Init.Window = 80;
  hwwdg.Init.Counter = 127;
  hwwdg.Init.EWIMode = WWDG_EWI_DISABLE;
  if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN WWDG_Init 2 */

  /* USER CODE END WWDG_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_RED_Pin|LED_ORANGE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_RED_Pin LED_ORANGE_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin|LED_ORANGE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* =========================================================
 * TASK 1: SensorRead (Priority HIGH) — Adaptive sampling
 * ========================================================= */
static void Task_SensorRead(void const *arg)
{
    SensorData_t data;
    uint16_t prev_s1 = 0;
    uint32_t delay_ms = 10;

    for (;;)
    {
        while (!dma_done_flag) { osThreadYield(); }
        dma_done_flag = 0;

        data.volt_s1_mv   = adc_to_mv(adc_dma_buf[0]);
        data.volt_s2_mv   = adc_to_mv(adc_dma_buf[1]);
        data.state        = sys_state;
        data.timestamp_ms = HAL_GetTick();

        /* Feed WWDG — wajib dalam window */
        HAL_WWDG_Refresh(&hwwdg);

        /* Adaptive sampling: delta besar ? rate tinggi */
        uint16_t raw_s1 = adc_dma_buf[0];
        uint16_t delta = (raw_s1 > prev_s1) ? (raw_s1 - prev_s1)
                                             : (prev_s1 - raw_s1);
        if (delta > 50)       delay_ms = 10;
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
                while (1) {}  /* WWDG timeout ? hardware reset */
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
/* USER CODE END 4 */

/* USER CODE BEGIN Header_Task_SafetyCheck */
/**
  * @brief  Function implementing the SafetyTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Task_SafetyCheck */
void Task_SafetyCheck(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Task_SensorRead */
/**
* @brief Function implementing the SensorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_SensorRead */
void Task_SensorRead(void const * argument)
{
  /* USER CODE BEGIN Task_SensorRead */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_SensorRead */
}

/* USER CODE BEGIN Header_Task_DataProcess */
/**
* @brief Function implementing the ProcessTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_DataProcess */
void Task_DataProcess(void const * argument)
{
  /* USER CODE BEGIN Task_DataProcess */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_DataProcess */
}

/* USER CODE BEGIN Header_Task_CommOutput */
/**
* @brief Function implementing the CommTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_CommOutput */
void Task_CommOutput(void const * argument)
{
  /* USER CODE BEGIN Task_CommOutput */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_CommOutput */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
