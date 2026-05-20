/**
 * @file    main_f401.h
 * @brief   Header untuk main_f401.c — STM32F401VE
 */
#ifndef MAIN_F401_H
#define MAIN_F401_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cmsis_os.h"

/* FreeRTOS Queue */
osMessageQDef(SensorDataQueue, 8, uint32_t);

/* ── Pin Mapping STM32F401VE ─────────────────────────────── */
/* ADC Inputs (Analog — tidak perlu GPIO init manual) */
#define SENSOR1_ADC_CH     ADC_CHANNEL_0   /* PA0 */
#define SENSOR2_ADC_CH     ADC_CHANNEL_1   /* PA1 */

/* PWM Output */
#define PWM_PIN            GPIO_PIN_8      /* PA8 — TIM1_CH1 (AF1) */
#define PWM_GPIO_PORT      GPIOA

/* UART */
#define UART_TX_PIN        GPIO_PIN_9      /* PA9  — USART1_TX (AF7) */
#define UART_RX_PIN        GPIO_PIN_10     /* PA10 — USART1_RX (AF7) */
#define UART_GPIO_PORT     GPIOA

/* I2C */
#define I2C_SCL_PIN        GPIO_PIN_6      /* PB6 — I2C1_SCL (AF4) */
#define I2C_SDA_PIN        GPIO_PIN_7      /* PB7 — I2C1_SDA (AF4) */
#define I2C_GPIO_PORT      GPIOB

/* LED Indicators */
#define LED_GREEN_PIN      GPIO_PIN_12     /* PD12 */
#define LED_ORANGE_PIN     GPIO_PIN_13     /* PD13 */
#define LED_RED_PIN        GPIO_PIN_14     /* PD14 */
#define LED_GPIO_PORT      GPIOD

#ifdef __cplusplus
}
#endif

#endif /* MAIN_F401_H */
