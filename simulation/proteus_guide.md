# Panduan Simulasi Proteus — STM32F401VE (Alternatif F407)

> **Catatan Perubahan**: STM32F407VGTx tidak tersedia di Proteus. Diganti dengan **STM32F401VE**
> yang tersedia di library Proteus (CM4_). Kedua MCU berbasis ARM Cortex-M4 dan
> kompatibel untuk tujuan simulasi metode ini.

## Perbandingan F407VGTx vs F401VE

| Fitur | STM32F407VGTx (rencana) | STM32F401VE (pengganti) | Impact |
|-------|------------------------|------------------------|--------|
| Core | Cortex-M4 @ 168 MHz | Cortex-M4 @ **84 MHz** | Clock config berbeda |
| Flash | 1 MB | 512 KB | Cukup untuk kode |
| RAM | 192 KB + 64 KB CCM | 96 KB (tanpa CCM) | Hapus CCM section |
| ADC | ADC1/2/3 | ADC1 | Cukup (pakai ADC1) |
| TIM1 | Ada | Ada | Sama |
| USART1 | PA9/PA10 | PA9/PA10 | **Sama persis** |
| I2C1 | PB6/PB7 | PB6/PB7 | **Sama persis** |
| WWDG | Ada | Ada | Sama |
| DMA2 | Stream0 CH0 | Stream0 CH0 | **Sama persis** |
| GPIO PD12-15 | Ada (LQFP100) | Ada (LQFP100 VE) | **Sama persis** |

---

## Daftar Komponen Proteus

| Komponen | Part Number Proteus | Pin MCU | Fungsi |
|----------|---------------------|---------|--------|
| MCU | **STM32F401VE** | — | Mikrokontroler utama |
| Potensiometer 1 | POT-HG (10kΩ) | **PA0** | Simulasi Sensor 1 (ADC CH0) |
| Potensiometer 2 | POT-HG (10kΩ) | **PA1** | Simulasi Sensor 2 (ADC CH1) |
| LED Hijau | LED-GREEN | **PD12** | Indikator Normal |
| LED Orange | LED-YELLOW | **PD13** | Indikator Warning |
| LED Merah | LED-RED | **PD14** | Indikator Fault |
| Terminal Virtual | VIRTUAL TERMINAL | **PA9** (TX) / **PA10** (RX) | Output UART log CSV |
| Osiloskop | OSCILLOSCOPE | **PA8** | Sinyal PWM TIM1_CH1 |
| LCD I2C 16x2 | LM016L + PCF8574 | **PB6** (SCL) / **PB7** (SDA) | Display status sensor |
| Resistor LED | RES (330Ω) × 3 | Seri ke PD12/13/14 | Current limiter LED |
| Resistor I2C | RES (4.7kΩ) × 2 | PB6→VCC, PB7→VCC | Pull-up I2C (wajib!) |
| Power Supply | POWER (+3.3V, GND) | — | Catu daya |

---

## Skema Koneksi (Sesuai Kode main_f401.c)

```
STM32F401VE
│
├── PA0 (ANALOG) ──── Wiper POT1 (Sensor 1, 0–3.3V)
│                    POT1: pin kiri = GND, pin kanan = +3.3V
│
├── PA1 (ANALOG) ──── Wiper POT2 (Sensor 2, 0–3.3V)
│                    POT2: pin kiri = GND, pin kanan = +3.3V
│
├── PA8 (AF1/TIM1_CH1) ────── Oscilloscope CH-A  (PWM 10 kHz)
│
├── PA9  (AF7/USART1_TX) ─── VIRTUAL TERMINAL: RXD  ← MCU TX
├── PA10 (AF7/USART1_RX) ─── VIRTUAL TERMINAL: TXD  → MCU RX
│
├── PB6 (AF4/I2C1_SCL) ──[4.7kΩ]── +3.3V
│                       ──────────── PCF8574 SCL
├── PB7 (AF4/I2C1_SDA) ──[4.7kΩ]── +3.3V
│                       ──────────── PCF8574 SDA
│                       PCF8574 → LM016L (LCD 16x2)
│                       PCF8574 I2C address: 0x27 (A0=A1=A2=GND)
│
├── PD12 ──[330Ω]── LED Hijau  Anode ── GND   (STATE_NORMAL)
├── PD13 ──[330Ω]── LED Orange Anode ── GND   (STATE_WARNING)
└── PD14 ──[330Ω]── LED Merah  Anode ── GND   (STATE_FAULT)
```

> **PENTING — GPIO Alternate Function**: Pin PA8, PA9, PA10, PB6, PB7
> **harus dikonfigurasi** sebagai Alternate Function di `MX_GPIO_Init()` agar
> peripheral TIM1, USART1, dan I2C1 dapat dikendalikan MCU. Kode `main_f401.c`
> sudah diperbaiki (v2) — pastikan menggunakan versi terbaru.

---

## Konfigurasi GPIO Per Peripheral (Referensi Kode)

| Pin | AF | Mode | Pull | Peripheral |
|-----|----|------|------|------------|
| PA0 | — | Analog | No Pull | ADC1 CH0 |
| PA1 | — | Analog | No Pull | ADC1 CH1 |
| PA8 | AF1 | AF_PP | No Pull | TIM1_CH1 (PWM) |
| PA9 | AF7 | AF_PP | Pull-Up | USART1_TX |
| PA10 | AF7 | AF_PP | Pull-Up | USART1_RX |
| PB6 | AF4 | AF_OD | Pull-Up | I2C1_SCL |
| PB7 | AF4 | AF_OD | Pull-Up | I2C1_SDA |
| PD12 | — | Output_PP | No Pull | LED Normal |
| PD13 | — | Output_PP | No Pull | LED Warning |
| PD14 | — | Output_PP | No Pull | LED Fault |

---

## Langkah Setup Proteus

### 1. Buat Project Baru
1. Buka Proteus → **File > New Project**
2. Template: *"Default Schematic Only"*
3. Simpan: `simulation/proteus_schematic.pdsprj`

### 2. Tambah Komponen (Pick Device = P)
Cari dan tambahkan satu per satu:
- `STM32F401VE` → letakkan di tengah/kiri
- `POT-HG` × 2 (10kΩ)
- `LED-GREEN`, `LED-YELLOW`, `LED-RED`
- `RES` × 3 nilai **330Ω** (current limiter LED)
- `RES` × 2 nilai **4.7kΩ** (pull-up I2C ke +3.3V)
- `LM016L` + `PCF8574` (LCD 16x2 via I2C)
- Dari **Instruments**: `VIRTUAL TERMINAL`, `OSCILLOSCOPE`
- Dari **Power**: tambahkan `VCC` (+3.3V) dan `GND`

### 3. Hubungkan Komponen
Sesuai skema koneksi di atas. Tips:
- Gunakan **Net Label** (Place > Net Label) untuk koneksi panjang agar rapi
- Klik kanan pin MCU → "Place Wire"
- Pastikan polaritas LED: **Anode ke resistor**, **Cathode ke GND**
- Pull-up 4.7kΩ I2C: satu ujung ke PB6/PB7, ujung lain ke +3.3V

### 4. Konfigurasi PCF8574 (LCD I2C)
1. Double-click komponen `PCF8574`
2. Pastikan **A0 = A1 = A2 = GND** → address = **0x27**
3. Hubungkan VCC ke +5V, GND ke GND
4. SCL → PB6 (STM32), SDA → PB7 (STM32)

### 5. Load File HEX
1. Double-click komponen **STM32F401VE**
2. Field **"Program File"**: browse ke `src/MDK-ARM/Debug/AdaptiveSafetySensor.hex`
3. **Crystal Frequency**: `8MHz`
4. Klik OK

### 6. Konfigurasi Virtual Terminal
1. Double-click Virtual Terminal
2. **Baud Rate**: `115200` | **Data Bits**: `8` | **Parity**: `None` | **Stop Bits**: `1`
3. Pastikan **RXD terhubung ke PA9** (MCU TX → Terminal RX)

### 7. Jalankan dan Amati
1. Klik **Play (▶)**
2. Putar POT1 dan POT2 untuk mengubah nilai sensor
3. Amati:
   - **LED**: PD12 (Hijau=Normal), PD13 (Orange=Warning), PD14 (Merah=Fault)
   - **Virtual Terminal**: data CSV setiap ~100ms
   - **Oscilloscope**: sinyal PWM 10 kHz berubah duty cycle

---

## Format Data CSV dari Virtual Terminal

```
timestamp_ms,s1_mv,s2_mv,pwm_pct,state
1000,1650,1720,51,0
2000,2900,2850,88,1
3000,3100,3200,98,2
...
```
- **state 0** = NORMAL (LED Hijau, PD12)
- **state 1** = WARNING (LED Orange, PD13)
- **state 2** = FAULT (LED Merah PD14, sistem reset via WWDG)

Threshold:
- NORMAL: 200 mV ≤ V1,V2 ≤ 2800 mV
- WARNING: salah satu sensor keluar range (fault_counter 1–2)
- FAULT: fault_counter ≥ 3 → WWDG timeout → hardware reset

---

## Bug yang Diperbaiki di main_f401.c (v2)

| Bug | Sebelum (Salah) | Sesudah (Benar) |
|-----|-----------------|-----------------|
| GPIO AF PA8 hilang | Tidak ada config PA8 | `GPIO_AF1_TIM1`, `AF_PP` |
| GPIO AF PA9/PA10 hilang | Tidak ada config UART pins | `GPIO_AF7_USART1`, `AF_PP`, Pull-Up |
| GPIO AF PB6/PB7 hilang | Tidak ada config I2C pins | `GPIO_AF4_I2C1`, `AF_OD`, Pull-Up |
| Field ADC salah (F1 style) | `DMAConversionMode = ADC_DMAACCESSMODE_1` | `DMAContinuousRequests = ENABLE` |

---

## Troubleshooting

| Masalah | Solusi |
|---------|--------|
| STM32F401VE tidak ada | Cari "STM32F401" di Pick Device, pilih VE |
| Virtual Terminal kosong | Pastikan PA9 → RXD terminal, baud 115200, cek AF7 di kode |
| LED tidak menyala | Cek resistor 330Ω, polaritas LED, pin PD12/13/14 |
| PWM tidak terlihat di osiloskop | Pastikan PA8 terhubung, input osiloskop di channel A |
| LCD tidak tampil | Cek pull-up 4.7kΩ di PB6/PB7, PCF8574 A0=A1=A2=GND |
| HEX tidak berjalan | Pastikan build Keil 0 error, gunakan `main_f401.c` bukan `main_backup.c` |
| PWM duty tidak berubah | Putar POT melewati setengah putaran (threshold 200–2800 mV) |
| Fault langsung muncul | POT di posisi 0 atau max = fault, putar ke tengah dulu |
