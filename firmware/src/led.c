#include "led.h"

uint32_t LED_FLAG = 0;

led_pwm_struct led_pwm_reg = {
        .cmd = led_pwm_page,
        .addr = led_first_addr_data
//      .data =
        // cs1_sw1, cs2_sw1, cs3_sw1,
        // ...
        // cs16_sw1, cs17_sw1, cs18_sw1,
        // cs1_sw2, cs2_sw2, cs3_sw2,
        // ...
        // cs16_sw11, cs17_sw11, cs18_sw11
};

led_scale_struct led_scale_reg = {
        .cmd = led_scale_page,
        .addr = led_first_addr_data
};


led_func_struct led_func_reg = {
        .cmd = led_func_page,
        .addr = led_first_addr_func,
        .data = 0x09,  // addr 0x00 conf reg: ssd normal, osde disable, d3 1, sws sw1-sw11
        led_gcc,  // addr 0x01: global current
//        0x00,  // addr 0x02: pd, pu res default, not test yet
//        0x36, 0x00,  // default pwm freq 29kHz
};

uint8_t *led_data_pointer[2] = {led_pwm_reg.data, led_scale_reg.data};


uint8_t led_color[3] = {}, led_color_flag = 0, led_delay = 0, led_effect = 0;

void leds_test() {
//    *(led_pwm_data_pointer + led_key[0][b]) = 1;
    led_color[r] = 0x80;
    led_color[g] = 0x80;
    led_color[b] = 0x80;
//    leds_colors(pwm);
    leds_colors(pwm, led_color[r], led_color[g], led_color[b]);
    led_color[r] = 0xff;
    led_color[g] = 0xff;
    led_color[b] = 0xff;
//    leds_colors(scale);
    leds_colors(scale, led_color[r], led_color[g], led_color[b]);
//    led_effect = LED_STATIC;
    led_effect = LED_SPECTRUM;
//    led_effect = LED_REACTIVE;
}



void leds_colors(uint8_t mode, uint8_t r_val, uint8_t g_val, uint8_t b_val) {

    for (int i = 0; i < KB_ROW_NUM; ++i) {
//        *(led_data_pointer[mode] + led_key[i][r]) = r_val;
//        *(led_data_pointer[mode] + led_key[i][g]) = g_val;
//        *(led_data_pointer[mode] + led_key[i][b]) = b_val;
        for (int j = 0; j < KB_COL_NUM; ++j) {
            *(led_data_pointer[mode] + led_key[i][j][r]) = r_val;
            *(led_data_pointer[mode] + led_key[i][j][g]) = g_val;
            *(led_data_pointer[mode] + led_key[i][j][b]) = b_val;
        }
    }

    for (int i = 0; i < led_e_num; ++i) {
        *(led_data_pointer[mode] + led_edg[i][r]) = r_val;
        *(led_data_pointer[mode] + led_edg[i][g]) = g_val;
        *(led_data_pointer[mode] + led_edg[i][b]) = b_val;
    }

    for (int i = 0; i < led_s_num; ++i) {
        *(led_data_pointer[mode] + led_sys[i][r]) = r_val;
        *(led_data_pointer[mode] + led_sys[i][g]) = g_val;
        *(led_data_pointer[mode] + led_sys[i][b]) = b_val;
    }
}

void leds_color(uint8_t mode, uint8_t color, uint8_t color_val) {
    for (int i = 0; i < KB_ROW_NUM ; ++i) {
//        *(led_data_pointer[mode] + led_key[i][color]) = color_val;
        for (int j = 0; j < KB_COL_NUM; ++j) {
            *(led_data_pointer[mode] + led_key[i][j][color]) = color_val;
        }
    }
    for (int i = 0; i < led_e_num; ++i) {
        *(led_data_pointer[mode] + led_edg[i][color]) = color_val;
    }
    for (int i = 0; i < led_s_num; ++i) {
        *(led_data_pointer[mode] + led_sys[i][color]) = color_val;
    }
}

void led_colors_set(uint8_t mode, uint8_t r_val, uint8_t g_val, uint8_t b_val, const uint8_t *led_which) {
    *(led_data_pointer[mode] + led_which[r]) = r_val;
    *(led_data_pointer[mode] + led_which[g]) = g_val;
    *(led_data_pointer[mode] + led_which[b]) = b_val;
}

void led_color_set(uint8_t mode, uint8_t color, uint8_t val, const uint8_t *led_which) {
    *(led_data_pointer[mode] + led_which[color]) = val;
}

void leds_spectrum() {
    static uint8_t led_count = 0;
    if (led_count == 49) {
        led_count = 0;
        LED_FLAG |= LED_FLAG_CHANGE;
        switch (led_color_flag) {
            case 0:
                if (led_color[b] != 0xff) {
                    ++led_color[b];
                    leds_colors(pwm, led_color[r], led_color[g], led_color[b]);
                } else {
                    led_color_flag |= 1 << 0;
                }
                break;
            case 1:
                if (led_color[g] != 0xff) {
                    ++led_color[g];
                    leds_colors(pwm, led_color[r], led_color[g], led_color[b]);
                } else {
                    led_color_flag |= 1 << 1;
                }
                break;
            case 3:
                if (led_color[b] != 0x00) {
                    --led_color[b];
                    leds_colors(pwm, led_color[r], led_color[g], led_color[b]);
                } else {
                    led_color_flag &= ~(1 << 0);
                }
                break;
            case 2:
                if (led_color[r] != 0xff) {
                    ++led_color[r];
                    leds_colors(pwm, led_color[r], led_color[g], led_color[b]);
                } else {
                    led_color_flag |= 1 << 2;
                }
                break;
            case 6:
                if (led_color[g] != 0x00) {
                    --led_color[g];
                    leds_colors(pwm, led_color[r], led_color[g], led_color[b]);
                } else {
                    led_color_flag &= ~(1 << 1);
                }
                break;
            case 4:
                if (led_color[b] != 0xff) {
                    ++led_color[b];
                    leds_colors(pwm, led_color[r], led_color[g], led_color[b]);
                } else {
                    led_color_flag |= 1 << 0;
                }
                break;
            case 5:
                if (led_color[r] != 0x00) {
                    --led_color[r];
                    leds_colors(pwm, led_color[r], led_color[g], led_color[b]);
                } else {
                    led_color_flag &= ~(1 << 2);
                }
                break;

            default:
                break;
        }
    } else {
        ++led_count;
    }
}

void leds_reactive() {
    static uint8_t r_def = 0, g_def = 0, b_def = 0, r_val = 0xff, g_val = 0xff, b_val = 0xff;
    LED_FLAG |= LED_FLAG_CHANGE;

    for (int i = 0; i < KB_ROW_NUM; ++i) {
        if (kb_key_state[i][kb_col_num] == KB_PRESS) {
            led_colors_set(pwm, r_def, g_val, b_def, led_key[i][kb_col_num]);
        } else {
            led_colors_set(pwm, r_def, g_def, b_def, led_key[i][kb_col_num]);
        }
    }

    if (kb_key_count != 0) {
        led_colors_set(pwm, r_def, g_val, b_def, led_sys[0]);
    } else {
        led_colors_set(pwm, r_def, g_def, b_def, led_sys[0]);
    }

//    uint8_t test0 = 0, test1 = 1;
//    for (int i = 0; i < HID_KB_SEND_SIZE; ++i) {
//        test0 |= kb_buf_master[i];
//        test1 |= kb_usb_buf[i];
//    }
//    if (test0 != 0) {
//        led_colors_set(pwm, r_def, g_val, b_def, led_sys[1]);
//    } else {
//        led_colors_set(pwm, r_def, g_def, b_def, led_sys[1]);
//    }
//    if (test1 != 0) {
//        led_colors_set(pwm, r_def, g_val, b_def, led_sys[2]);
//    } else {
//        led_colors_set(pwm, r_def, g_def, b_def, led_sys[2]);
//    }

}

void leds_init() {
    leds_test();
    leds_on();
//    leds_off();
    spi_config();
//    spi_enable(SPI1);
    leds_dma();
}

void leds_on() {
    gpio_bit_set(LED_SDB_PORT, LED_SDB_PIN);
    LED_FLAG |= LED_FLAG_CHANGE;
    KB_CTL |= KB_CTL_LED;
    // wait until I2C bus is idle
//    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
}

void leds_off() {
    gpio_bit_reset(LED_SDB_PORT, LED_SDB_PIN);
    KB_CTL &= ~KB_CTL_LED;
//    LED_FLAG &= ~LED_FLAG_CHANGE;
    // wait until I2C bus is idle
//    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
}




void led_polling() {
    if (KB_CTL & KB_CTL_LED) {
//        static uint8_t led_count = 0;
//        if (led_count == 99) {
//            led_count = 0;

        switch (led_effect) {
            case LED_SPECTRUM:
                leds_spectrum();
//                LED_FLAG |= LED_FLAG_CHANGE;
                break;

            case LED_REACTIVE:
                leds_reactive();
//                LED_FLAG |= LED_FLAG_CHANGE;
                break;

            default:
//                leds_test();
                break;
        }
//        } else {
//            led_count++;
//        }

        led_dma(pwm);

//        led_dma_start(pwm);
    }
}

void led_dma(uint8_t mode) {
    static bool led_dma_flag = 0;

    if (LED_FLAG & LED_FLAG_CHANGE) {
        if (!led_dma_flag) {
            dma_config(mode);
            gpio_bit_write(GPIOB, GPIO_PIN_12, 0);
            dma_channel_enable(DMA0, DMA_CH4);
            led_dma_flag = 1;
        }

//        while (!dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));
        if (dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF) && (led_dma_flag == 1)) {
            dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_FTF);
            gpio_bit_write(GPIOB, GPIO_PIN_12, 1);
            dma_channel_disable(DMA0, DMA_CH4);
            led_dma_flag = 0;
            LED_FLAG &= ~LED_FLAG_CHANGE;
        }
    }
}

//void led_dma_start(uint8_t mode) {
//    if (LED_FLAG & LED_FLAG_CHANGE) {
////        if (led_dma_flag == 0) {
//        dma_config(mode);
//        gpio_bit_write(GPIOB, GPIO_PIN_12, 0);
////        dma_interrupt_enable(DMA0, DMA_CH4, DMA_INT_FTF);
//        dma_channel_enable(DMA0, DMA_CH4);
////            led_dma_flag = 1;
//        }
//}

//void led_dma_finish() {
////    if ((RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE)) && (SET == spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS))) {
//        gpio_bit_write(GPIOB, GPIO_PIN_12, 1);
//        dma_channel_disable(DMA0, DMA_CH4);
////        dma_interrupt_disable(DMA0, DMA_CH4, DMA_INT_FTF);
////    led_dma_flag = 0;
//        LED_FLAG &= ~LED_FLAG_CHANGE;
////    }
//
//}

//    uint8_t i2c_transmitter[i2c_trans_size] = {
//        0xfe, 0xc5,  // conf other reg: unlock cmd reg
//        0xfd, 0x04,  // conf cmd reg: reg pg4
//            0x00, 0x01,  // conf reg: ssd normal, osde disable, lgc 0, sws 9
//            0x01, 0x40,  // global current 64
////            0x02, 0x00,  // pd, pu res
//            0x36, 0x00,  // pwm freq 29kHz
//
//        0xfe, 0xc5,  // conf other reg unlock cmd reg
//        0xfd, 0x01, // write page 0
//            0x58, 0xff,  // sw9 cs29: pwm 255
//            0x57, 0x00,  // sw9 cs28: pwm 255
//            0x3a, 0xff,  // sw8 cs29: pwm 255
//            0x39, 0xff,  // sw8 cs28: pwm 255
//            0x1c, 0xff,  // sw7 cs29: pwm 255
//            0x1b, 0xff,  // sw7 cs28: pwm 255
//
//        0xfe, 0xc5,  // conf other reg unlock cmd reg
//        0xfd, 0x03, // write page 2
//            0x58, 0xff,  // sw9 cs29: scale 255
//            0x57, 0xff,  // sw9 cs28: scale 255
//            0x3a, 0xff,  // sw8 cs29: scale 255
//            0x39, 0xff,  // sw8 cs28: scale 255
//            0x1c, 0xff,  // sw7 cs29: scale 255
//            0x1b, 0xff,  // sw7 cs28: scale 255
//    };


/*void led_i2c_trans(uint8_t *led_i2c_trans_buf, uint8_t led_i2c_trans_size){
    // send a start condition to I2C bus
    i2c_start_on_bus(I2C1);
    // wait until SBSEND bit is set, hw
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    // send slave address to I2C bus, reset sbsend
    i2c_master_addressing(I2C1, IS31FL3741_ADDR_GND, I2C_TRANSMITTER);
    // wait until ADDSEND bit is set, hw
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));  // stuck
    // clear ADDSEND bit
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    for(uint8_t i = 0; i < led_i2c_trans_size; ++i){
        // wait until the transmit data buffer is empty
        // wait until the TBE bit is set
        while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));  // stuck
        // data transmission
        i2c_data_transmit(I2C1, led_i2c_trans_buf[i]);
    }
    // send a stop condition to I2C bus
    i2c_stop_on_bus(I2C1);
    // wait until stop condition generate
    while(I2C_CTL0(I2C1)&0x0200);
}*/

/*
void leds_func_trans() {
    led_i2c_trans(cmd_reg_unlock, cmd_reg_unlock_size);
    led_i2c_trans(reg_sel[func], reg_sel_size);
    led_i2c_trans(func_reg, func_reg_size);
}

void leds_pwm_trans() {
    led_i2c_trans(cmd_reg_unlock, cmd_reg_unlock_size);
    led_i2c_trans(reg_sel[pwm0], reg_sel_size);
    led_i2c_trans(led_reg_page0[pwm], led_reg_page0_size);

    led_i2c_trans(cmd_reg_unlock, cmd_reg_unlock_size);
    led_i2c_trans(reg_sel[pwm1], reg_sel_size);
    led_i2c_trans(led_reg_page1[pwm], led_reg_page1_size);
}

void leds_scale_trans() {
    led_i2c_trans(cmd_reg_unlock, cmd_reg_unlock_size);
    led_i2c_trans(reg_sel[scale0], reg_sel_size);
    led_i2c_trans(led_reg_page0[scale], led_reg_page0_size);

    led_i2c_trans(cmd_reg_unlock, cmd_reg_unlock_size);
    led_i2c_trans(reg_sel[scale1], reg_sel_size);
    led_i2c_trans(led_reg_page1[scale], led_reg_page1_size);
}

void leds_all_trans() {
    leds_pwm_trans();
    leds_scale_trans();
    leds_func_trans();
}
*/

static const uint8_t led_key[KB_ROW_NUM][KB_COL_NUM][3] = {
    LED_KEY0_B, LED_KEY0_G, LED_KEY0_R,
    LED_KEY1_B, LED_KEY1_G, LED_KEY1_R,
    LED_KEY2_B, LED_KEY2_G, LED_KEY2_R,
    LED_KEY3_B, LED_KEY3_G, LED_KEY3_R,
    LED_KEY4_B, LED_KEY4_G, LED_KEY4_R,
    LED_KEY5_B, LED_KEY5_G, LED_KEY5_R,
    LED_KEY6_B, LED_KEY6_G, LED_KEY6_R,
    LED_KEY7_B, LED_KEY7_G, LED_KEY7_R,
    LED_KEY8_B, LED_KEY8_G, LED_KEY8_R,
    LED_KEY9_B, LED_KEY9_G, LED_KEY9_R,
    LED_KEY10_B, LED_KEY10_G, LED_KEY10_R,
    LED_KEY11_B, LED_KEY11_G, LED_KEY11_R,
    LED_KEY12_B, LED_KEY12_G, LED_KEY12_R,
    LED_KEY13_B, LED_KEY13_G, LED_KEY13_R,
    LED_KEY14_B, LED_KEY14_G, LED_KEY14_R,
    LED_KEY15_B, LED_KEY15_G, LED_KEY15_R,
    LED_KEY16_B, LED_KEY16_G, LED_KEY16_R,
    LED_KEY17_B, LED_KEY17_G, LED_KEY17_R,
    LED_KEY18_B, LED_KEY18_G, LED_KEY18_R,
    LED_KEY19_B, LED_KEY19_G, LED_KEY19_R,
    LED_KEY20_B, LED_KEY20_G, LED_KEY20_R,
    LED_KEY21_B, LED_KEY21_G, LED_KEY21_R,
    LED_KEY22_B, LED_KEY22_G, LED_KEY22_R,
    LED_KEY23_B, LED_KEY23_G, LED_KEY23_R,
    LED_KEY24_B, LED_KEY24_G, LED_KEY24_R,
    LED_KEY25_B, LED_KEY25_G, LED_KEY25_R,
    LED_KEY26_B, LED_KEY26_G, LED_KEY26_R,
    LED_KEY27_B, LED_KEY27_G, LED_KEY27_R,
    LED_KEY28_B, LED_KEY28_G, LED_KEY28_R,
    LED_KEY29_B, LED_KEY29_G, LED_KEY29_R,
    LED_KEY30_B, LED_KEY30_G, LED_KEY30_R,
    LED_KEY31_B, LED_KEY31_G, LED_KEY31_R,
    LED_KEY32_B, LED_KEY32_G, LED_KEY32_R,
    LED_KEY33_B, LED_KEY33_G, LED_KEY33_R,
    LED_KEY34_B, LED_KEY34_G, LED_KEY34_R,
    LED_KEY35_B, LED_KEY35_G, LED_KEY35_R,
    LED_KEY36_B, LED_KEY36_G, LED_KEY36_R,
    LED_KEY37_B, LED_KEY37_G, LED_KEY37_R,
    LED_KEY38_B, LED_KEY38_G, LED_KEY38_R,
    LED_KEY39_B, LED_KEY39_G, LED_KEY39_R,
    LED_KEY40_B, LED_KEY40_G, LED_KEY40_R,
    LED_KEY41_B, LED_KEY41_G, LED_KEY41_R
};

static const uint8_t led_edg[led_e_num][3] = {
    LED_EDG0_B, LED_EDG0_G, LED_EDG0_R,
    LED_EDG1_B, LED_EDG1_G, LED_EDG1_R,
    LED_EDG2_B, LED_EDG2_G, LED_EDG2_R,
    LED_EDG3_B, LED_EDG3_G, LED_EDG3_R,
    LED_EDG4_B, LED_EDG4_G, LED_EDG4_R,
    LED_EDG5_B, LED_EDG5_G, LED_EDG5_R,
    LED_EDG6_B, LED_EDG6_G, LED_EDG6_R,
    LED_EDG7_B, LED_EDG7_G, LED_EDG7_R,
    LED_EDG8_B, LED_EDG8_G, LED_EDG8_R,
    LED_EDG9_B, LED_EDG9_G, LED_EDG9_R,
    LED_EDG10_B, LED_EDG10_G, LED_EDG10_R,
    LED_EDG11_B, LED_EDG11_G, LED_EDG11_R,
    LED_EDG12_B, LED_EDG12_G, LED_EDG12_R,
    LED_EDG13_B, LED_EDG13_G, LED_EDG13_R,
    LED_EDG14_B, LED_EDG14_G, LED_EDG14_R,
    LED_EDG15_B, LED_EDG15_G, LED_EDG15_R,
    LED_EDG16_B, LED_EDG16_G, LED_EDG16_R,
    LED_EDG17_B, LED_EDG17_G, LED_EDG17_R
};

static const uint8_t led_sys[led_s_num][3] = {
    LED_SYS0_B, LED_SYS0_G, LED_SYS0_R,
    LED_SYS1_B, LED_SYS1_G, LED_SYS1_R,
    LED_SYS2_B, LED_SYS2_G, LED_SYS2_R,
    LED_SYS3_B, LED_SYS3_G, LED_SYS3_R
};