# Safety-Critical Multi-Sensor Acquisition Framework with Priority-Deadline Hybrid Scheduling and Hybrid Memory Optimization on STM32

> A novel integrated embedded system approach on STM32F4 series combining 16 future research directions from peer-reviewed Scopus/WoS journal articles into a single, simulatable system.

## Table of Contents
- [Background](#background)
- [Proposed Method](#proposed-method)
- [Simulation Setup](#simulation-setup)
- [Results](#results)
- [Advantages](#advantages)
- [Authors](#authors)

## Background

This project addresses the challenge of building a **safety-critical, energy-efficient, and modular embedded data acquisition system** on the STM32F407VGTx (ARM Cortex-M4). The system is designed through a systematic literature review of **16 Scopus/WoS-indexed journal articles**, extracting and integrating future research directions across six topics:

1. Controller technology evolution
2. Microcontroller fundamentals — RTOS, Hypervisor, TEE
3. Microcontroller architecture — ARM Cortex-M, LLVM, TCM
4. Peripherals — Adaptive ADC, Multi-Protocol Gateway
5. Embedded programming — C framework, FreeRTOS DVFS, SOLID patterns
6. Safety-critical systems — CBF, AI safety standards

## Proposed Method

The proposed system integrates four core architectural innovations:

1. **Adaptive ADC Sampling** — DMA-based multi-channel ADC with a level-crossing trigger to dynamically reduce sampling rate during quiescent signal periods, saving CPU bandwidth and energy.
2. **Priority-Deadline Hybrid Scheduler** — FreeRTOS configured with 4 tasks using combined priority + deadline awareness. The safety-check task runs at CRITICAL priority; communication task runs at LOW priority, allowing energy-saving idle periods.
3. **Hybrid CCM/Cache Memory Placement** — Time-critical interrupt handlers and ADC DMA buffers are placed in the STM32F407's 64 KB CCM SRAM (zero-wait-state, equivalent to TCM), while general data uses the regular D-Cache.
4. **Safety Barrier Monitor** — A simplified Control Barrier Function (CBF) threshold checker runs every 500 ms. If sensor values breach safety limits, the Window Watchdog (WWDG) is intentionally starved, triggering a hardware reset and logging the fault.

### Block Diagram & Flowchart
Detailed architectural diagrams and program flowcharts can be found in the `docs/` directory.

## Simulation Setup

| Component | Details |
|-----------|---------|
| **MCU** | STM32F401VE (Proteus Simulation Equivalent) |
| **Code Generator** | STM32CubeMX |
| **IDE / Compiler** | Keil µVision 5 (MDK-ARM) |
| **Simulator** | Proteus 8.x |
| **Data Plotting** | GNUPlot 5.x |

### How to Run

1. Open `simulation/proteus_schematic.pdsprj` in Proteus.
2. Double click the STM32 component and load the compiled `.hex` file from `src/MDK-ARM/AdaptiveSafetySensor/`.
3. Run the simulation.
4. Observe results on the Virtual Terminal (UART logging) and Digital Oscilloscope (PWM Output).

## Advantages

| Aspect | Existing Methods | Proposed Method |
|--------|-----------------|-----------------|
| ADC Sampling | Fixed rate, wastes CPU bandwidth | Adaptive rate (level-crossing trigger) |
| Task Scheduling | Pure priority OR pure deadline | Hybrid priority-deadline (FreeRTOS) |
| Memory Usage | All data in general SRAM | Critical ISR buffer in CCM (TCM, 0 wait-state) |
| Safety Mechanism | Watchdog only | Watchdog + CBF threshold + Fault FSM |
| Serial Communication | Single protocol | Multi-protocol: UART + I2C + SPI |
| Firmware Structure | Ad-hoc monolithic C | SOLID-based modular C framework |

## Authors

| Name | Class |
|------|-------|
| Abigail | 4B |

**Course**: Pemrograman Kontroller  
**Department**: Rekayasa Teknologi Instrumentasi
