# Adaptive Safety-Critical Multi-Sensor Acquisition Framework with Priority-Deadline Hybrid Scheduling and Hybrid Memory Optimization on STM32F407VGTx

> A novel integrated embedded system approach on STM32F407VGTx combining 16 future research directions from peer-reviewed Scopus/WoS journal articles (2021–2026) into a single, simulatable system.

## Table of Contents
- [Background](#background)
- [Proposed Method](#proposed-method)
- [Simulation Setup](#simulation-setup)
- [Results](#results)
- [Advantages](#advantages)
- [Authors](#authors)
- [References](#references)

## Background

<!-- TODO: Describe the research gap identified from the 15 journal papers -->
<!-- Explain what problem your method addresses -->

This project addresses the challenge of building a **safety-critical, energy-efficient, and modular embedded data acquisition system** on the STM32F407VGTx (ARM Cortex-M4 @ 168 MHz). The system is designed through a systematic literature review of **16 Scopus/WoS-indexed journal articles (2021–2026)**, extracting and integrating future research directions across six topics:

1. Controller technology evolution (FW1–FW4)
2. Microcontroller fundamentals — RTOS, Hypervisor, TEE (FW5–FW7)
3. Microcontroller architecture — ARM Cortex-M, LLVM, TCM (FW8–FW9)
4. Peripherals — Adaptive ADC, Multi-Protocol Gateway (FW10–FW11)
5. Embedded programming — C framework, FreeRTOS DVFS, SOLID patterns (FW12–FW14)
6. Safety-critical systems — CBF, AI safety standards (FW15–FW16)

## Proposed Method

<!-- TODO: Describe your novel method that combines FW1–FW15 -->

### Block Diagram

<!-- TODO: Replace with your actual block diagram -->
![Block Diagram](docs/block_diagram.png)

### Flowchart

<!-- TODO: Replace with your actual flowchart -->
![Flowchart](docs/flowchart.png)

### Method Description

The proposed system integrates four core architectural innovations on STM32F407VGTx:

1. **Adaptive ADC Sampling (FW3, FW10)** — DMA-based multi-channel ADC with a level-crossing trigger to dynamically reduce sampling rate during quiescent signal periods, saving CPU bandwidth and energy.

2. **Priority-Deadline Hybrid Scheduler (FW6, FW13)** — FreeRTOS configured with 4 tasks using combined priority + deadline awareness. The safety-check task runs at CRITICAL priority with a 0.5 ms deadline; communication task runs at LOW priority, allowing energy-saving idle periods.

3. **Hybrid CCM/Cache Memory Placement (FW9)** — Time-critical interrupt handlers and ADC DMA buffers are placed in the STM32F407's 64 KB CCM SRAM (zero-wait-state, equivalent to TCM), while general data uses the regular D-Cache.

4. **Safety Barrier Monitor (FW15, FW16)** — A simplified Control Barrier Function (CBF) threshold checker runs every 500 ms. If sensor values breach safety limits for 3 consecutive checks, the Window Watchdog (WWDG) is intentionally starved, triggering a hardware reset and logging the fault.

## Simulation Setup

| Component | Details |
|-----------|---------|
| **MCU** | STM32F407VGTx |
| **Code Generator** | STM32CubeMX |
| **IDE / Compiler** | Keil µVision 5 (MDK-ARM) |
| **Simulator** | Proteus 8.x |
| **Data Plotting** | GNUPlot 5.x |

### How to Run

1. Open `simulation/proteus_schematic.pdsprj` in Proteus
2. Load the compiled `.hex` file from `src/` into the STM32 component
3. Run the simulation
4. Observe results on the virtual terminal and oscilloscope
5. Export data to CSV for plotting

### Circuit Schematic

<!-- TODO: Add Proteus schematic screenshot -->
![Circuit](simulation/screenshots/schematic.png)

## Results

<!-- TODO: Add your GNUPlot results -->
<!-- ![Result 1](data/plots/plot1.png) -->

### Data Analysis

<!-- TODO: Describe and analyze your simulation data -->

## Advantages

<!-- TODO: Compare your method with existing approaches from the journals -->

| Aspect | Existing Methods | Proposed Method |
|--------|-----------------|-----------------|
| ADC Sampling | Fixed rate, wastes CPU bandwidth | Adaptive rate (level-crossing trigger) |
| Task Scheduling | Pure priority OR pure deadline | Hybrid priority-deadline (FreeRTOS) |
| Memory Usage | All data in general SRAM | Critical ISR buffer in CCM (TCM, 0 wait-state) |
| Safety Mechanism | Watchdog only | Watchdog + CBF threshold + Fault FSM |
| Serial Communication | Single protocol | Multi-protocol: UART + I2C + SPI |
| Firmware Structure | Ad-hoc monolithic C | SOLID-based modular C framework |
| Energy Efficiency | No management | Clock gating during idle + adaptive ADC |

## Authors

<!-- TODO: Fill in your info -->
| Name | NIM | Class |
|------|-----|-------|
|      |     | 4B    |
|      |     | 4B    |

**Course**: Pemrograman Kontroller  
**Semester**: Genap 2025/2026  
**Department**: Rekayasa Teknologi Instrumentasi

## References

See the full journal summary table in [references/journal_summary.md](references/journal_summary.md).

<!-- TODO: Add your 15 references in proper citation format -->
