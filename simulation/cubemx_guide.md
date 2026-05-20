# Panduan STM32CubeMX — STM32F401VETx

## Overview

CubeMX digunakan untuk:
1. Memilih MCU dan mengkonfigurasi pin
2. Mengatur clock tree (84 MHz)
3. Mengaktifkan peripheral (ADC, TIM, UART, I2C, WWDG, DMA, FreeRTOS)
4. Generate kode inisialisasi (HAL) → buka di Keil µVision

---

## STEP 1 — Buat Project Baru

1. Buka **STM32CubeMX**
2. Klik **"ACCESS TO MCU SELECTOR"**
3. Di kotak pencarian ketik: `STM32F401VE`
4. Pilih **STM32F401VETx** dari hasil pencarian
5. Klik **"Start Project"**

---

## STEP 2 — Konfigurasi Pin (Pinout & Configuration)

Tab aktif: **Pinout & Configuration**

### 2.1 ADC1 — Sensor Input

1. Di panel kiri: **Analog → ADC1**
2. Centang: **IN0** dan **IN1**
3. Setting di bagian kanan:
   - **Scan Conversion Mode**: Enabled
   - **Continuous Conversion Mode**: Enabled
   - **Number Of Conversion**: 2
   - Rank 1 → Channel: `Channel 0`, Sampling Time: `84 Cycles`
   - Rank 2 → Channel: `Channel 1`, Sampling Time: `84 Cycles`
   - **DMA Continuous Requests**: Enabled

> PA0 dan PA1 otomatis dikonfigurasi sebagai **ADC1_IN0** dan **ADC1_IN1**

### 2.2 TIM1 — PWM Output

1. Panel kiri: **Timers → TIM1**
2. **Clock Source**: Internal Clock
3. **Channel 1**: PWM Generation CH1
4. Setting Parameter:
   - **Prescaler**: `0`
   - **Counter Period (ARR)**: `8399`
   - **Pulse (CCR1)**: `0`
   - **PWM Mode**: PWM mode 1

> PA8 otomatis dikonfigurasi sebagai **TIM1_CH1**

### 2.3 USART1 — Data Logging

1. Panel kiri: **Connectivity → USART1**
2. **Mode**: Asynchronous
3. Parameter:
   - **Baud Rate**: `115200`
   - **Word Length**: 8 Bits
   - **Parity**: None
   - **Stop Bits**: 1

> PA9 (TX) dan PA10 (RX) otomatis terkonfigurasi

### 2.4 I2C1 — Display LCD

1. Panel kiri: **Connectivity → I2C1**
2. **I2C**: I2C
3. Parameter:
   - **I2C Speed Mode**: Fast Mode
   - **I2C Clock Speed**: `400000` Hz

> PB6 (SCL) dan PB7 (SDA) otomatis terkonfigurasi

### 2.5 WWDG — Window Watchdog

1. Panel kiri: **System Core → WWDG**
2. **Activated**: ✅ centang
3. Parameter:
   - **WWDG counter clock prescaler**: 8
   - **WWDG window value**: `80` (hex: 0x50)
   - **Initial WWDG counter value**: `127` (hex: 0x7F)
   - **Early wakeup interrupt**: Disabled

### 2.6 GPIO — LED Indikator

Pada tampilan pinout utama (kanan):
1. Klik pin **PD12** → pilih **GPIO_Output**
2. Klik pin **PD13** → pilih **GPIO_Output**
3. Klik pin **PD14** → pilih **GPIO_Output**

Lalu di panel kiri: **System Core → GPIO**
Untuk masing-masing PD12/13/14:
- **GPIO output level**: Low
- **GPIO mode**: Output Push Pull
- **GPIO Pull-up/Pull-down**: No pull-up and no pull-down
- **Maximum output speed**: Low
- **User Label**: `LED_GREEN` / `LED_ORANGE` / `LED_RED`

### 2.7 DMA — ADC1 Transfer

1. Panel kiri: **System Core → DMA**
2. Tab **DMA2**
3. Klik **"Add"** → pilih **ADC1**
4. Setting:
   - **Direction**: Peripheral To Memory
   - **Priority**: High
   - **Mode**: Circular
   - **Data Width Peripheral**: Half Word
   - **Data Width Memory**: Half Word
   - **Increment Address Memory**: ✅

---

## STEP 3 — Clock Configuration

Klik tab **Clock Configuration** (di bagian atas)

Setting clock tree:
```
Input Frequency: 8 MHz (HSE)
    │
    ├── PLL Source Mux: HSE
    ├── PLLM: 8
    ├── PLLN: 336
    ├── PLLP: /4   ← PENTING! (bukan /2)
    │
    └── System Clock Mux: PLLCLK
            │
            └── SYSCLK = 84 MHz
                    │
                    ├── AHB Prescaler: /1  → HCLK = 84 MHz
                    ├── APB1 Prescaler: /2 → PCLK1 = 42 MHz
                    └── APB2 Prescaler: /1 → PCLK2 = 84 MHz
```

**Cara cepat**: Di kotak **"HCLK (MHz)"** ketik `84` lalu tekan Enter — CubeMX akan auto-calculate semua prescaler.

> ⚠️ Pastikan tidak ada tanda merah/orange. Jika ada, sesuaikan prescaler APB1 menjadi `/2`.

---

## STEP 4 — FreeRTOS (Middleware)

1. Panel kiri: **Middleware → FREERTOS**
2. **Interface**: CMSIS_V1
3. Tab **Tasks and Queues**:

Hapus default task, lalu **Add** 4 task berikut:

| Task Name | Priority | Stack Size | Entry Function |
|-----------|----------|------------|----------------|
| SafetyTask | osPriorityRealtime | 256 | Task_SafetyCheck |
| SensorTask | osPriorityHigh | 256 | Task_SensorRead |
| ProcessTask | osPriorityNormal | 512 | Task_DataProcess |
| CommTask | osPriorityLow | 512 | Task_CommOutput |

4. Tab **Config parameters**:
   - **USE_TIMERS**: Disabled (tidak perlu)
   - **TOTAL_HEAP_SIZE**: `10240` (10 KB — cukup untuk 4 task)

---

## STEP 5 — Project Settings

Klik tab **Project Manager**

### 5.1 Project
- **Project Name**: `AdaptiveSafetySensor`
- **Project Location**: `C:\...\ETS-Bagian-B\src\`
- **Application Structure**: Advanced
- **Toolchain/IDE**: **MDK-ARM V5** (Keil µVision 5)

### 5.2 Code Generator
- ✅ **Copy only the necessary library files**
- ✅ **Generate peripheral initialization as a pair of '.c/.h' files per peripheral**
- ✅ **Keep User Code when re-generating**

### 5.3 Advanced Settings
- Semua peripheral biarkan **HAL** (default)

---

## STEP 6 — Generate Code

1. Klik tombol **"GENERATE CODE"** (pojok kanan atas)
2. Tunggu proses selesai
3. Klik **"Open Project"** → Keil µVision akan terbuka otomatis

---

## STEP 7 — Integrasi Kode di Keil µVision

Setelah project terbuka di Keil:

### 7.1 Tambahkan Logic ke main.c

CubeMX membuat `main.c` dengan struktur:
```c
/* USER CODE BEGIN ... */
/* USER CODE END   ... */
```

**Salin** isi dari `src/main_f401.c` ke bagian yang sesuai:

| Dari `main_f401.c` | Ke section `main.c` Keil |
|--------------------|--------------------------|
| `#include` tambahan | `/* USER CODE BEGIN Includes */` |
| Definisi konstanta & struct | `/* USER CODE BEGIN PD */` |
| Variabel global | `/* USER CODE BEGIN PV */` |
| Prototype task functions | `/* USER CODE BEGIN PFP */` |
| `osThreadCreate` calls | `/* USER CODE BEGIN RTOS_THREADS */` |
| Isi function Task_* | `/* USER CODE BEGIN 4 */` |
| Helper functions | `/* USER CODE BEGIN 4 */` |
| HAL callbacks | `/* USER CODE BEGIN 4 */` |

> ⚠️ Jangan modifikasi kode DI LUAR area `USER CODE BEGIN/END` —
> akan ditimpa jika regenerate dari CubeMX.

### 7.2 Build Project

1. **Project → Build Target** (F7)
2. Target: **0 Error(s), 0 Warning(s)**
3. File HEX ada di: `src/MDK-ARM/AdaptiveSafetySensor/AdaptiveSafetySensor.hex`

### 7.3 Jika Ada Error Umum

| Error | Solusi |
|-------|--------|
| `FreeRTOS.h not found` | Pastikan FreeRTOS diaktifkan di CubeMX, regenerate |
| `FLASH_LATENCY_2 undefined` | Pastikan pilih `stm32f4xx_hal_conf.h` yang benar |
| `osMessageQDef redefined` | Hapus duplikat dari `main_f401.h`, biarkan CubeMX yang generate |
| `undefined reference to Task_*` | Pastikan semua function Task_ ada di `/* USER CODE BEGIN 4 */` |

---

## Ringkasan Peripheral yang Diaktifkan

```
STM32F401VETx @ 84 MHz
│
├── ADC1      : CH0 (PA0) + CH1 (PA1), Continuous, Scan, DMA
├── DMA2      : Stream0, CH0, Circular, Half-Word, untuk ADC1
├── TIM1      : CH1 PWM (PA8), 10 kHz, ARR=8399
├── USART1    : TX=PA9, RX=PA10, 115200 bps, Async
├── I2C1      : SCL=PB6, SDA=PB7, 400 kHz Fast Mode
├── WWDG      : Prescaler=8, Window=0x50, Counter=0x7F
├── GPIO OUT  : PD12 (LED_GREEN), PD13 (LED_ORANGE), PD14 (LED_RED)
└── FreeRTOS  : CMSIS_V1, 4 Tasks, Heap=10KB
```
