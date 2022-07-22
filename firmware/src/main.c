/*!
    \file    main.c
    \brief   USB main routine for HID device(USB keyboard)

    \version 2020-08-04, V1.1.0, firmware for GD32VF103
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "drv_usb_hw.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "main.h"

const uint32_t KEY_ROW_GPIO_PIN[KB_ROW_NUM] = {
        KEY_ROW_0_GPIO_PIN, KEY_ROW_1_GPIO_PIN, KEY_ROW_2_GPIO_PIN,
        KEY_ROW_3_GPIO_PIN, KEY_ROW_4_GPIO_PIN, KEY_ROW_5_GPIO_PIN};
const uint32_t KEY_COL_GPIO_PIN[KB_COL_NUM] = {
        KEY_COL_0_GPIO_PIN, KEY_COL_1_GPIO_PIN, KEY_COL_2_GPIO_PIN,KEY_COL_3_GPIO_PIN,
        KEY_COL_4_GPIO_PIN, KEY_COL_5_GPIO_PIN,KEY_COL_6_GPIO_PIN};
const uint32_t KEY_COL_GPIO_PORT[KB_COL_NUM] = {
        KEY_COL_0_GPIO_PORT, KEY_COL_1_GPIO_PORT, KEY_COL_2_GPIO_PORT,KEY_COL_3_GPIO_PORT,
        KEY_COL_4_GPIO_PORT, KEY_COL_5_GPIO_PORT, KEY_COL_6_GPIO_PORT};

usb_dev usb_device;

//extern hid_composite_fop_handler fop_handler;

#ifdef KB_CTL_ADC
uint16_t adc_value[10] = {}, adc_char[1000] = {};
#endif

int main(void)
{
    eclic_config();

    rcu_config();

    usb_rcu_config();

    usb_timer_init();

//    hid_composite_itfop_register(&usb_device, &fop_handler);

    gpio_config();

    #ifdef KB_CTL_ADC
        KB_CTL |= KB_CTL_USB_CDC;
        dma_config(11);
        adc_config();
    #else
        kb_init();
        leds_init();
        kb_uart_init();
    #endif

    kb_init_sync();
    systick_config();
    usb_intr_config();

    /* test if user code is programmed starting from address 0x08010000 */
//    if (0x0001AAB1 == (*(__IO uint32_t*)APP_LOADED_ADDR)) {
//    clear_csr(mstatus, MSTATUS_MIE);
//    asm volatile ("jr %0 " :: "r"(APP_LOADED_ADDR));
//    }

    if (KB_CTL & KB_CTL_USB_KB) {
        usbd_init(&usb_device, USB_CORE_ENUM_FS, &hid_kb_desc, &hid_kb_cb);
    } else if (KB_CTL & KB_CTL_USB_COMPOSITE) {
        usbd_init(&usb_device, USB_CORE_ENUM_FS, &usb_composite_desc, &usb_composite_cb);
    } else if (KB_CTL & KB_CTL_USB_CDC) {
        usbd_init(&usb_device, USB_CORE_ENUM_FS, &cdc_desc, &cdc_class);

    }
    while (USBD_CONFIGURED != usb_device.dev.cur_status);

    if (KB_CTL & KB_CTL_USB_KB || KB_CTL & KB_CTL_USB_COMPOSITE || KB_CTL & KB_CTL_USB_CDC) {
        KB_CTL |= KB_CTL_USB;
    }


    while (1) {
    }
}

void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
//    rcu_periph_clock_enable(RCU_I2C1);

    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_SPI1);
    rcu_periph_clock_enable(RCU_DMA0);
//    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_USART2);

    rcu_periph_clock_enable(RCU_ADC0);
//    rcu_periph_clock_enable(RCU_ADC1);

    // 12 MHz
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);
}

void gpio_config(void)
{
    /* key gpio */
    for (int i = 0; i < KB_COL_NUM; ++i) {
        gpio_init(KEY_COL_GPIO_PORT[i], GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, KEY_COL_GPIO_PIN[i]);
        gpio_bit_reset(KEY_COL_GPIO_PORT[i],KEY_COL_GPIO_PIN[i]);
    }

#ifdef KB_CTL_ADC
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ,
              GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
#else
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,
              GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
#endif

    /* I2C0 and I2C1 GPIO ports */
    /* connect PB10 to I2C1_SCL */
    /* connect PB11 to I2C1_SDA */
//    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);

    // spi1 hard nss
//    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15);
    // sp1 soft nss
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_bit_write(GPIOB, GPIO_PIN_12, 1);
//    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

    // is31f3743b shutdown pin
    gpio_init(LED_SDB_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_SDB_PIN);

    // jtag remap gpio
//    gpio_pin_remap_config(GPIO_SWJ_DISABLE_REMAP, ENABLE);
//    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
//    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
//
//    gpio_bit_write(GPIOA, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, 0);
//    gpio_bit_write(GPIOB, GPIO_PIN_3 | GPIO_PIN_4, 0);


    // usart0
//    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
//    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    // usart2
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);


}

void dma_config(uint8_t which)
{
    dma_parameter_struct dma_init_struct;
    switch (which) {
        case pwm:
            /* configure SPI0 transmit DMA : DMA0, DMA_CH2 */
            dma_deinit(DMA0, DMA_CH4);
            dma_struct_para_init(&dma_init_struct);

            dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI1);
            dma_init_struct.memory_addr  = (uint32_t)&led_pwm_reg;
            dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
            dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
            dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
            dma_init_struct.priority     = DMA_PRIORITY_LOW;
            dma_init_struct.number       = led_data_size;
            dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
            dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
            dma_init(DMA0, DMA_CH4, &dma_init_struct);
            /* configure DMA mode */
            dma_circulation_disable(DMA0, DMA_CH4);
            dma_memory_to_memory_disable(DMA0, DMA_CH4);

//            dma_interrupt_enable(DMA0, DMA_CH4, DMA_INT_FTF);
            break;

        case scale:
            /* configure SPI0 transmit DMA : DMA0, DMA_CH2 */
            dma_deinit(DMA0, DMA_CH4);
            dma_struct_para_init(&dma_init_struct);

            dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI1);
            dma_init_struct.memory_addr  = (uint32_t)&led_scale_reg;
            dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
            dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
            dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
            dma_init_struct.priority     = DMA_PRIORITY_LOW;
            dma_init_struct.number       = led_data_size;
            dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
            dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
            dma_init(DMA0, DMA_CH4, &dma_init_struct);
            /* configure DMA mode */
            dma_circulation_disable(DMA0, DMA_CH4);
            dma_memory_to_memory_disable(DMA0, DMA_CH4);

//            dma_interrupt_enable(DMA0, DMA_CH4, DMA_INT_FTF);
            break;
        case func:
            /* configure SPI0 transmit DMA : DMA0, DMA_CH2 */
            dma_deinit(DMA0, DMA_CH4);
            dma_struct_para_init(&dma_init_struct);

            dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI1);
            dma_init_struct.memory_addr  = (uint32_t)&led_func_reg;
            dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
            dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
            dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
            dma_init_struct.priority     = DMA_PRIORITY_LOW;
            dma_init_struct.number       = led_func_size;
            dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
            dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
            dma_init(DMA0, DMA_CH4, &dma_init_struct);
            /* configure DMA mode */
            dma_circulation_disable(DMA0, DMA_CH4);
            dma_memory_to_memory_disable(DMA0, DMA_CH4);

//            dma_interrupt_enable(DMA0, DMA_CH4, DMA_INT_FTF);
            break;

//        case 12:
            /* configure SPI0 receive DMA : DMA0, DMA_CH1 */
            //    dma_deinit(DMA0, DMA_CH3);
            //    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI1);
            //    dma_init_struct.memory_addr  = (uint32_t)spi1_receive_array;
            //    dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
            //    dma_init_struct.priority     = DMA_PRIORITY_HIGH;
            //    dma_init(DMA0, DMA_CH3, &dma_init_struct);
                        /* configure DMA mode */
            //    dma_circulation_disable(DMA0, DMA_CH3);
            //    dma_memory_to_memory_disable(DMA0, DMA_CH3);

        case 4:
            /* deinitialize DMA channel3(USART0 tx) */
            dma_deinit(DMA0, DMA_CH1);
            dma_struct_para_init(&dma_init_struct);

            dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
            dma_init_struct.memory_addr = (uint32_t)kb_buf_master;
            dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
            dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
            dma_init_struct.number = HID_KB_SEND_SIZE;
            dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);
            dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
            dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
            dma_init_struct.priority = DMA_PRIORITY_HIGH;
            dma_init(DMA0, DMA_CH1, &dma_init_struct);
            /* configure DMA mode */
            dma_circulation_disable(DMA0, DMA_CH1);
            dma_memory_to_memory_disable(DMA0, DMA_CH1);
//            dma_circulation_enable(DMA0, DMA_CH1);
            dma_channel_enable(DMA0, DMA_CH1);
            break;
        case 5:
            /* deinitialize DMA channel4 (USART0 rx) */
            dma_deinit(DMA0, DMA_CH2);
            dma_struct_para_init(&dma_init_struct);

            dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
            dma_init_struct.memory_addr = (uint32_t)kb_buf_slave;
            dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
            dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
            dma_init_struct.number = HID_KB_SEND_SIZE;
            dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);
            dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
            dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
            dma_init_struct.priority = DMA_PRIORITY_HIGH;
            dma_init(DMA0, DMA_CH2, &dma_init_struct);
            /* configure DMA mode */
            dma_circulation_disable(DMA0, DMA_CH2);
            dma_memory_to_memory_disable(DMA0, DMA_CH2);
//            dma_circulation_enable(DMA0, DMA_CH2);
            dma_channel_enable(DMA0, DMA_CH2);
            break;

        #ifdef KB_CTL_ADC
        case 11:
            /* ADC DMA_channel configuration */
            dma_deinit(DMA0, DMA_CH0);
            dma_struct_para_init(&dma_init_struct);

            /* initialize DMA data mode */
            dma_init_struct.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
            dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
            dma_init_struct.memory_addr  = (uint32_t)(adc_value);
            dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
            dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
            dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;
            dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
            dma_init_struct.number       = 1;
            dma_init_struct.priority     = DMA_PRIORITY_HIGH;
            dma_init(DMA0, DMA_CH0, &dma_init_struct);
            dma_circulation_enable(DMA0, DMA_CH0);
            /* enable DMA channel */
            dma_channel_enable(DMA0, DMA_CH0);
            break;
    #endif
    }

}

//void i2c_config(void)
//{
//    /* I2C clock configure */
//    i2c_clock_config(I2C1, 1000000, I2C_DTCY_16_9);
//    /* I2C address configure */
//    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_OWN_ADDRESS7);
//    /* enable I2C1 */
//    i2c_enable(I2C1);
//    /* enable acknowledge */
//    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
//}


void spi_config(void)
{
    spi_parameter_struct spi_init_struct;
    /* deinitialize SPI and the parameters */
    spi_i2s_deinit(SPI1);
    spi_struct_para_init(&spi_init_struct);

    /* configure SPI0 parameters */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_4;  // 48 / 4 = 12.
    spi_init_struct.endian               = SPI_ENDIAN_MSB;

    spi_init(SPI1, &spi_init_struct);

    spi_crc_off(SPI1);
    spi_nss_output_disable(SPI1);
    spi_nss_internal_high(SPI1);

    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
    spi_enable(SPI1);

//    spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_TBE);

}

void usart_config() {
    usart_deinit(USART2);
    usart_baudrate_set(USART2, 2000000U);
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);

    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(USART2, USART_DENT_ENABLE);
    usart_dma_receive_config(USART2, USART_DENR_ENABLE);

    usart_enable(USART2);

//    dma_interrupt_enable(DMA0, DMA_CH1, DMA_INT_FTF);
//    dma_channel_enable(DMA0, DMA_CH1);

//    dma_interrupt_enable(DMA0, DMA_CH2, DMA_INT_FTF);
//    dma_channel_enable(DMA0, DMA_CH2);

}

void eclic_config(void)
{
    eclic_global_interrupt_enable();
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL2_PRIO2);

//    eclic_irq_enable(CLIC_INT_TMR, 3, 3);
//    eclic_irq_enable(CLIC_INT_TMR, 1, 3);
//    eclic_irq_enable(DMA0_Channel1_IRQn, 1, 0);
//    eclic_irq_enable(DMA0_Channel2_IRQn, 1, 0);
//    eclic_irq_enable(DMA0_Channel3_IRQn, 1, 0);
//    eclic_irq_enable(DMA0_Channel4_IRQn, 1, 0);
//    eclic_irq_enable(TIMER1_IRQn,1,2);
//    eclic_irq_enable(TIMER3_IRQn,1,1);
}

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void kb_timer_config(void)
{
    /* ----------------------------------------------------------------------------
    TIMER1 Configuration:
    TIMER1CLK = SystemCoreClock/96 = 1MHz, the period is us (125 / 1 * 10^6 s = 125us). on 1kHz usb. > 15us
    TIMER1CLK = SystemCoreClock/48 = 2MHz, the period is us (125 / 2 * 10^6 s = 62.5us).
    min TIMER1CLK = SystemCoreClock/12 = 8MHz, the period is us (125 / 8 * 10^6 s = 15.625us). on 8kHz usb.
    ---------------------------------------------------------------------------- */
//    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER1 configuration */
//    timer_initpara.prescaler         = 10799;
    timer_initpara.prescaler         = 95;
//    timer_initpara.prescaler         = 47;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
//    timer_initpara.period            = 99;
//    timer_initpara.period            = 39;
    timer_initpara.period            = 124;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);

//    timer_dma_enable(TIMER1, TIMER_DMA_UPD);
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    timer_enable(TIMER1);
}

void leds_dma() {
    dma_config(pwm);
    dma_channel_enable(DMA0, DMA_CH4);
    gpio_bit_write(GPIOB, GPIO_PIN_12, 0);
    // wait dma transmit complete
    while(!dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));
//    while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
    while(SET == spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS));
    gpio_bit_write(GPIOB, GPIO_PIN_12, 1);
    dma_channel_disable(DMA0, DMA_CH4);

    dma_config(scale);
    dma_channel_enable(DMA0, DMA_CH4);
    gpio_bit_write(GPIOB, GPIO_PIN_12, 0);
    // wait dma transmit complete
    while(!dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));
//    while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
    while(SET == spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS));
    gpio_bit_write(GPIOB, GPIO_PIN_12, 1);
    dma_channel_disable(DMA0, DMA_CH4);

    dma_config(func);
    dma_channel_enable(DMA0, DMA_CH4);
    gpio_bit_write(GPIOB, GPIO_PIN_12, 0);
    // wait dma transmit complete
    while(!dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));
//    while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
    while(SET == spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS));
    gpio_bit_write(GPIOB, GPIO_PIN_12, 1);
    dma_channel_disable(DMA0, DMA_CH4);
}

void dfu() {

}

//void keys_usart_dma_cfg() {
//
//    dma_config(4);
//    dma_config(5);

    /* wait DMA Channel transfer complete */
//    while(RESET == dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF));
//    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
//    while(RESET == usart_flag_get(USART2, USART_FLAG_TC));

    /* wait DMA channel transfer complete */
//    while(RESET == dma_flag_get(DMA0, DMA_CH2, DMA_FLAG_FTF));
//    printf("\n\r%s\n\r", spi1_receive_array);
//}


/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* reset ADC */
    adc_deinit(ADC0);
//    adc_deinit(ADC1);
    /* ADC mode config */
//    adc_mode_config(ADC_DAUL_REGULAL_PARALLEL);
    adc_mode_config(ADC_MODE_FREE);
    adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
    /* ADC continous function enable */
//    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
//    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
//    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);

    /* ADC data alignment config */
//    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
//    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
//    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);
//    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 2);
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1U);

    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_1POINT5);
//    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_1, ADC_SAMPLETIME_1POINT5);
//    adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_2, ADC_SAMPLETIME_1POINT5);
//    adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_3, ADC_SAMPLETIME_1POINT5);
//    adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_4, ADC_SAMPLETIME_1POINT5);
//    adc_regular_channel_config(ADC0, 5, ADC_CHANNEL_5, ADC_SAMPLETIME_1POINT5);

    /* ADC trigger config */
//    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_T1_CH1);
//    adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
//    adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    /* enable ADC interface */
//    adc_enable(ADC1);
//    delay_1ms(1);
    /* ADC calibration and reset calibration */
//    adc_calibration_enable(ADC1);

    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);

    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}