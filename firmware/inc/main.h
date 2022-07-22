#ifndef MAIN_H
#define MAIN_H

#include "keyboard.h"
#include "led.h"
#include "drv_usb_dev.h"
#include "usb_composite.h"
//#include "hid_keyboard_core.h"
#include "cdc_acm_core.h"
#include "systick.h"
//#include "dfu_core.h"
#include "riscv_encoding.h"
//#include "gd32vf103_it.h"

//#define KB_COL_NUM 7U
//#define KB_ROW_NUM 6U
#if !defined(KB_ROW_NUM) && !defined(KB_COL_NUM)
    #define KB_ROW_NUM 6U
    #define KB_COL_NUM 7U
#endif

#define KEY_ROW_GPIO_PORT GPIOA
#define KEY_ROW_0_GPIO_PIN GPIO_PIN_0
#define KEY_ROW_1_GPIO_PIN GPIO_PIN_1
#define KEY_ROW_2_GPIO_PIN GPIO_PIN_2
#define KEY_ROW_3_GPIO_PIN GPIO_PIN_3
#define KEY_ROW_4_GPIO_PIN GPIO_PIN_4
#define KEY_ROW_5_GPIO_PIN GPIO_PIN_5

#define KEY_COL_0_GPIO_PIN GPIO_PIN_7
#define KEY_COL_0_GPIO_PORT GPIOB
#define KEY_COL_1_GPIO_PIN GPIO_PIN_6
#define KEY_COL_1_GPIO_PORT GPIOB
#define KEY_COL_2_GPIO_PIN GPIO_PIN_5
#define KEY_COL_2_GPIO_PORT GPIOB
#define KEY_COL_3_GPIO_PIN GPIO_PIN_1
#define KEY_COL_3_GPIO_PORT GPIOB
#define KEY_COL_4_GPIO_PIN GPIO_PIN_0
#define KEY_COL_4_GPIO_PORT GPIOB
#define KEY_COL_5_GPIO_PIN GPIO_PIN_7
#define KEY_COL_5_GPIO_PORT GPIOA
#define KEY_COL_6_GPIO_PIN GPIO_PIN_6
#define KEY_COL_6_GPIO_PORT GPIOA

#define LED_SDB_PIN GPIO_PIN_4
#define LED_SDB_PORT GPIOB

const uint32_t KEY_ROW_GPIO_PIN[KB_ROW_NUM], KEY_COL_GPIO_PIN[KB_COL_NUM], KEY_COL_GPIO_PORT[KB_COL_NUM];
extern usb_dev usb_device;

void rcu_config(void);
void gpio_config(void);
//void i2c_config(void);
void spi_config(void);
void dma_config(uint8_t which);
void eclic_config();
void leds_dma();
void usart_config();
void adc_config();

#ifdef KB_CTL_ADC
extern uint16_t adc_value[10], adc_char[1000];
#endif

#endif //MAIN_H
