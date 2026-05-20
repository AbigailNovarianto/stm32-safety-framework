# Metode Baru — Perancangan Sistem

## Judul Metode

> **"Adaptive Safety-Critical Multi-Sensor Acquisition Framework with Priority-Deadline Hybrid Scheduling and Hybrid Memory Optimization on STM32F407VGTx"**
>
> *(Kerangka Kerja Akuisisi Multi-Sensor Safety-Critical Adaptif dengan Penjadwalan Hibrid Prioritas-Deadline dan Optimasi Memori Hybrid pada STM32F407VGTx)*

---

## Latar Belakang & Motivasi

Sistem embedded modern menghadapi tiga tantangan utama secara bersamaan:
1. **Keandalan akuisisi data** — sampling ADC dengan rate adaptif untuk menghemat energi sekaligus menjaga akurasi sinyal
2. **Penjadwalan task real-time** — kebutuhan memenuhi *hard deadline* dan *soft deadline* secara bersamaan tanpa kelaparan (starvation)
3. **Keselamatan sistem (safety)** — jaminan formal bahwa sistem tidak melanggar batas keamanan meskipun terjadi gangguan

Metode ini menggabungkan **16 future work** dari literatur menjadi satu sistem terintegrasi yang dapat disimulasikan pada **STM32F407VGTx** menggunakan **FreeRTOS + HAL**.

---

## Peta Kontribusi Future Work → Komponen Sistem

| Komponen Sistem | Sumber FW |
|-----------------|-----------|
| Adaptive ADC DMA dengan rate variabel | FW3, FW10 |
| Hybrid TCM (CCM) + Cache memory placement | FW9 |
| Priority-Deadline hybrid scheduler (FreeRTOS) | FW6, FW13 |
| Modular C firmware (SOLID principles) | FW12, FW14 |
| Multi-protocol gateway (UART + I2C + SPI) | FW11 |
| Safety barrier monitoring (Watchdog + threshold CBF) | FW15 |
| Secure logging & error state machine | FW7, FW16 |
| Analisis energi & DVFS-lite via clock gating | FW5, FW13 |
| Platform MCU (STM32F407VGTx — ARM Cortex-M4) | FW8, FW2 |
| Embedded AI inference kecil (opsional, FW4) | FW1, FW4 |

---

## Deskripsi Sistem

### Input
- **Sensor 1**: Sinyal analog (ADC CH0) — simulasi sensor suhu/tekanan via potensiometer
- **Sensor 2**: Sinyal analog (ADC CH1) — simulasi sensor kelembaban/arus via potensiometer
- **Sensor 3**: Sinyal digital (GPIO) — simulasi fault/alarm eksternal

### Proses (STM32F407VGTx)

```
┌─────────────────────────────────────────────────────────────┐
│                    STM32F407VGTx                            │
│                                                             │
│  ┌──────────────┐    ┌─────────────────┐    ┌───────────┐  │
│  │ DMA-ADC      │    │ FreeRTOS Tasks  │    │ Safety    │  │
│  │ Adaptive     │───▶│ Priority-       │───▶│ Monitor   │  │
│  │ Sampling     │    │ Deadline Hybrid │    │ (WWDG +   │  │
│  └──────────────┘    └─────────────────┘    │  CBF)     │  │
│         │                    │              └───────────┘  │
│         ▼                    ▼                    │         │
│  ┌──────────────┐    ┌─────────────────┐          │         │
│  │ CCM (TCM)    │    │ Protocol Stack  │          │         │
│  │ Critical ISR │    │ UART/I2C/SPI   │          │         │
│  │ Buffer       │    │ Multi-Protocol  │          │         │
│  └──────────────┘    └─────────────────┘          │         │
└─────────────────────────────────────────────────────────────┘
```

### Output
- **PWM** (TIM1) → Kontrol aktuator (motor/LED dimmer)
- **UART** → Logging data real-time ke terminal / PC
- **LED GPIO** → Indikator status (Normal / Warning / Fault)
- **I2C** → Komunikasi ke display / slave device

---

## Diagram Blok Sistem

```mermaid
graph TB
    subgraph INPUT["🔵 INPUT LAYER"]
        S1["Sensor 1\nAnalog ADC CH0\n(Suhu/Tekanan)"]
        S2["Sensor 2\nAnalog ADC CH1\n(Kelembaban/Arus)"]
        S3["Fault Signal\nDigital GPIO\n(Alarm Eksternal)"]
    end

    subgraph MCU["⚙️ STM32F407VGTx — Cortex-M4 @ 168 MHz"]
        direction TB

        subgraph ACQ["Akuisisi Data (FW3, FW10)"]
            DMA["DMA Controller\nCircular Mode"]
            ADC["ADC1 Multi-Channel\n12-bit, 2.4 MSPS"]
            ADAPT["Adaptive Sampling\nRate Controller\n(Level-Crossing Logic)"]
        end

        subgraph MEM["Manajemen Memori (FW9)"]
            CCM["CCM SRAM 64KB\nCritical ISR Buffer\n(TCM Equivalent)"]
            CACHE["D-Cache / I-Cache\nGeneral Data"]
        end

        subgraph SCHED["Penjadwalan Real-Time (FW6, FW13)"]
            RTOS["FreeRTOS Kernel"]
            T1["Task: SensorRead\nPriority=HIGH\nDeadline=1ms"]
            T2["Task: DataProcess\nPriority=MED\nDeadline=5ms"]
            T3["Task: SafetyCheck\nPriority=CRITICAL\nDeadline=0.5ms"]
            T4["Task: CommOutput\nPriority=LOW\nDeadline=10ms"]
        end

        subgraph SAFETY["Safety Monitor (FW15, FW16)"]
            WWDG["Window Watchdog\n(WWDG)"]
            CBF["Threshold Barrier\nChecker\n(simplified CBF)"]
            FSM["Fault State\nMachine\nNormal→Warning→Fault"]
        end

        subgraph PROTO["Multi-Protocol Gateway (FW11)"]
            UART["USART1\nData Logger\n115200 bps"]
            I2C["I2C1\nDisplay/Slave"]
            SPI["SPI1\nExternal Memory"]
        end
    end

    subgraph OUTPUT["🟢 OUTPUT LAYER"]
        PWM["TIM1 PWM\nAktuator Kontrol"]
        LED["LED Indikator\nNormal/Warning/Fault"]
        TERM["Terminal UART\nData Logging"]
        DISP["I2C Display\nReal-Time Status"]
    end

    S1 -->|"Vin 0-3.3V"| ADC
    S2 -->|"Vin 0-3.3V"| ADC
    S3 -->|"GPIO IRQ"| T3

    ADC --> DMA
    DMA --> ADAPT
    ADAPT -->|"Rate Control"| ADC
    ADAPT --> CCM

    CCM --> T1
    CACHE --> T2

    RTOS --> T1 & T2 & T3 & T4
    T3 --> CBF
    T3 --> WWDG
    CBF --> FSM
    FSM -->|"FAULT"| WWDG

    T1 --> T2
    T2 --> T4
    T4 --> UART & I2C & SPI

    T2 -->|"PWM Duty"| PWM
    FSM -->|"State Signal"| LED
    UART --> TERM
    I2C --> DISP
    PWM --> OUTPUT
```

---

## Flowchart Program Utama

```mermaid
flowchart TD
    START(["🟢 START\nSystem Boot"]) --> INIT

    INIT["Inisialisasi Hardware\n• Clock config 168 MHz HSE\n• GPIO, USART, I2C, SPI\n• ADC + DMA circular\n• TIM1 PWM\n• WWDG enable"] --> RTOS_INIT

    RTOS_INIT["Inisialisasi FreeRTOS\n• Buat 4 task\n• Set priority & stack\n• Buat Queue & Semaphore"] --> SCHED_START

    SCHED_START["xTaskScheduler Start"] --> PARALLEL

    PARALLEL{{"Scheduler Running\n[Parallel Tasks]"}}

    PARALLEL --> TASK_SENSOR
    PARALLEL --> TASK_SAFETY
    PARALLEL --> TASK_COMM

    subgraph TASK_SENSOR["Task: SensorRead (Priority HIGH)"]
        direction TB
        SR1["Tunggu DMA Complete\nCallback"] --> SR2
        SR2["Baca ADC Buffer\ndari CCM SRAM"] --> SR3
        SR3{"Adaptive Rate:\nDelta > threshold?"} -->|Ya| SR4
        SR3 -->|Tidak| SR5
        SR4["Tingkatkan\nSampling Rate"] --> SR6
        SR5["Turunkan\nSampling Rate"] --> SR6
        SR6["Kirim data ke Queue\nxQueueSend"] --> SR7
        SR7["Feed WWDG\n(kritis: harus dalam window)"] --> SR1
    end

    subgraph TASK_PROCESS["Task: DataProcess (Priority MED)"]
        direction TB
        DP1["xQueueReceive\nTunggu data sensor"] --> DP2
        DP2["Filter Moving Average\n(simple low-pass)"] --> DP3
        DP3["Hitung nilai fisik\n(ADC → Volt → Unit)"] --> DP4
        DP4["Update PWM duty\nHTIM1 PWM SetCompare"] --> DP5
        DP5["Simpan ke ring\nbuffer logging"] --> DP1
    end

    subgraph TASK_SAFETY["Task: SafetyCheck (Priority CRITICAL)"]
        direction TB
        SC1["Baca nilai sensor\nlangsung dari buffer"] --> SC2
        SC2{"Nilai dalam\nbatas aman?\n(CBF Check)"} -->|✅ Ya| SC3
        SC2 -->|❌ Tidak| SC4
        SC3["State = NORMAL\nLED Green\nvTaskDelay 500ms"] --> SC1
        SC4{"Sudah\nWarning?"} -->|Tidak| SC5
        SC4 -->|Ya, 3x berturut| SC6
        SC5["State = WARNING\nLED Yellow\nLog via UART"] --> SC1
        SC6["State = FAULT\nLED Red\nWWDG NOT fed\n→ System Reset"] --> RESET
        RESET(["🔴 SYSTEM RESET\nvia WWDG Timeout"])
    end

    subgraph TASK_COMM["Task: CommOutput (Priority LOW)"]
        direction TB
        CO1["Baca ring buffer\nlogging"] --> CO2
        CO2["Format string:\nCSV timestamp,s1,s2,pwm,state"] --> CO3
        CO3["Kirim via USART1\nHAL_UART_Transmit_DMA"] --> CO4
        CO4["Update I2C display\nHAL_I2C_Master_Transmit"] --> CO5
        CO5["vTaskDelay 100ms"] --> CO1
    end
```

---

## Keunggulan Metode (vs State-of-the-Art)

| Aspek | Metode Konvensional | Metode Baru |
|-------|--------------------|----|
| ADC Sampling | Fixed rate, boros energi | Adaptive rate (level-crossing) |
| Scheduling | Pure priority atau pure deadline | Hybrid priority-deadline |
| Memory | Semua di SRAM biasa | Critical path di CCM (TCM) |
| Safety | Hanya watchdog | Watchdog + CBF threshold + FSM |
| Komunikasi | Satu protokol | Multi-protocol (UART/I2C/SPI) |
| Firmware structure | Ad-hoc monolithic | SOLID-based modular |
| Energi | Tidak dikelola | Clock gating task idle |

---

## Mapping ke Periferal STM32F407VGTx

| Periferal | Fungsi | Config |
|-----------|--------|--------|
| ADC1 + DMA2 | Akuisisi multi-sensor | 2 channel, circular, 12-bit |
| CCM SRAM (0x10000000) | Buffer ADC ISR critical | 64 KB, zero-wait-state |
| TIM1 CH1 | PWM aktuator | 20 kHz, duty 0–100% |
| USART1 (PA9/PA10) | Data logging UART | 115200 bps, DMA TX |
| I2C1 (PB6/PB7) | Display / slave | 400 kHz Fast Mode |
| SPI1 (PA5/6/7) | External flash (opsional) | Mode 0, 10 MHz |
| WWDG | Window Watchdog safety | Window ~50ms |
| GPIO (PD12–PD15) | LED indikator (Discovery board) | Output PP |
| FreeRTOS | RTOS kernel | 4 tasks, configTICK_RATE_HZ=1000 |

---

## Deliverable Fase 2

- [x] Judul metode baru
- [x] Tabel mapping FW → komponen
- [x] Diagram blok (Mermaid)
- [x] Flowchart (Mermaid)
- [x] Mapping periferal STM32F407VGTx
- [ ] Ekspor diagram ke PNG (via browser/draw.io)
- [ ] Simpan ke `docs/block_diagram.png` dan `docs/flowchart.png`
