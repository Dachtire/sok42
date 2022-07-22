#include "keyboard.h"

uint8_t kb_buf_mix[HID_KB_SEND_SIZE] = {}, kb_buf_master[HID_KB_SEND_SIZE] = {}, kb_buf_slave[HID_KB_SEND_SIZE] = {}, kb_buf_empty[HID_KB_SEND_SIZE] = {};
uint8_t kb_buf_cntrl[HID_CNTLR_BUF_SIZE] = {};
uint8_t kb_buf_mouse[HID_MICE_BUF_SIZE] = {};

uint8_t kb_buf_recev[HID_KB_RECEV_SIZE];

uint8_t kb_col_num = 0, kb_row_num = 0;
uint8_t (*kb_layout)[KB_COL_NUM] = kb_lyrs[0];
bool kb_key_state[KB_ROW_NUM][KB_COL_NUM] = {};
uint32_t KB_CTL = 0;
uint32_t KB_FLAG;
uint8_t kb_key_count = 0;
uint8_t kb_cntlr_buf_num = 0, kb_cntlr_buf_tmp = 0;

uint8_t *kb_usb_buf = 0;

uint8_t kb_row_read = 0;

#if KB_SIDE == LEFT
    uint8_t kb_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM] = {
        // dovrak
        KEY_ESCAPE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_DELETE,
        KEY_GRAVE_ACCENT_AND_TILDE, KEY_1_EXCLAMATION_MARK, KEY_2_AT, KEY_3_NUMBER_SIGN, KEY_4_DOLLAR, KEY_5_PERCENT, KEY_HOME,
        KEY_TAB, KEY_SINGLE_AND_DOUBLE_QUOTE, KEY_COMMA_AND_LESS, KEY_DOT_GREATER, KEY_P, KEY_Y, KEY_PAGEUP,
        KEY_ENTER, KEY_A, KEY_O, KEY_E, KEY_U, KEY_I, KEY_PAGEDOWN,
        KEY_LEFT_SHIFT, KEY_SEMICOLON_COLON, KEY_Q, KEY_J, KEY_K, KEY_X, KEY_END,
        KEY_UPARROW, KEY_DOWNARROW, KEY_LEFTARROW, KEY_RIGHTARROW, KEY_LEFT_CONTROL, KEY_SPACEBAR, KEY_LEFT_ALT,

        // qwert
        KEY_ESCAPE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_PRINTSCREEN,
        KEY_GRAVE_ACCENT_AND_TILDE, KEY_1_EXCLAMATION_MARK, KEY_2_AT, KEY_3_NUMBER_SIGN, KEY_4_DOLLAR, KEY_5_PERCENT, KEY_HOME,
        KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_PAGEUP,
        KEY_ENTER, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_PAGEDOWN,
        KEY_LEFT_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_END,
        KEY_UPARROW, KEY_DOWNARROW, KEY_LEFTARROW, KEY_RIGHTARROW, KEY_LEFT_CONTROL, KEY_SPACEBAR, KEY_LEFT_ALT,
    };

    uint8_t kb_cntlr_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM] = {
        CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
        CNTLR_BACK, CNTLR_LEFT_STICK_BUTTON, CNTLR_LEFT_STICK_LEFT, CNTLR_LEFT_STICK_RIGHT, CNTLR_LEFT_STICK_UP, CNTLR_LEFT_STICK_DOWN, CNTLR_NONE,
        CNTLR_START, CNTLR_RIGHT_STICK_BUTTON, CNTLR_RIGHT_STICK_LEFT, CNTLR_RIGHT_STICK_RIGHT, CNTLR_RIGHT_STICK_UP, CNTLR_RIGHT_STICK_DOWN, CNTLR_NONE,
        CNTLR_RIGHT_BUMPER, CNTLR_RIGHT_TRIGGER, CNTLR_A, CNTLR_B, CNTLR_X, CNTLR_Y, CNTLR_NONE,
        CNTLR_LEFT_BUMPER, CNTLR_LEFT_TRIGGER, CNTLR_DPAD_LEFT, CNTLR_DPAD_RIGHT, CNTLR_DPAD_UP, CNTLR_DPAD_DOWN, CNTLR_NONE,
        CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
    };
#else
    uint8_t kb_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM] = {
        KEY_F12, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11,
        KEY_OBRACKET_AND_OBRACE, KEY_6_CARET, KEY_7_AMPERSAND, KEY_8_ASTERISK, KEY_9_OPARENTHESIS, KEY_0_CPARENTHESIS, KEY_CBRACKET_AND_CBRACE,
        KEY_EQUAL_PLUS, KEY_F, KEY_G, KEY_C, KEY_R, KEY_L, KEY_BACKSLASH_VERTICAL_BAR,
        KEY_MINUS_UNDERSCORE, KEY_D, KEY_H, KEY_T, KEY_N, KEY_S, KEY_BACKSPACE,
        KEY_SLASH_QUESTION, KEY_B, KEY_M, KEY_W, KEY_V, KEY_Z, KEY_RIGHT_SHIFT,
        KEY_RIGHT_ALT, KEY_SPACEBAR, KEY_RIGHT_CONTROL, KEY_RIGHT_GUI, KEY_INSERT, KEY_PRINTSCREEN, KEY_CAPS_LOCK,

        KEY_F12, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11,
        KEY_OBRACKET_AND_OBRACE, KEY_6_CARET, KEY_7_AMPERSAND, KEY_8_ASTERISK, KEY_9_OPARENTHESIS, KEY_0_CPARENTHESIS, KEY_CBRACKET_AND_CBRACE,
        KEY_EQUAL_PLUS, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_BACKSLASH_VERTICAL_BAR,
        KEY_MINUS_UNDERSCORE, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON_COLON, KEY_BACKSPACE,
        KEY_SLASH_QUESTION, KEY_N, KEY_M, KEY_COMMA_AND_LESS, KEY_DOT_GREATER, KEY_SINGLE_AND_DOUBLE_QUOTE, KEY_RIGHT_SHIFT,
        KEY_RIGHT_ALT, KEY_SPACEBAR, KEY_RIGHT_CONTROL, KEY_DELETE, KEY_RIGHT_GUI, KEY_INSERT, KEY_CAPS_LOCK,
    };

    uint8_t kb_cntlr_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM] = {
        CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
        CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
        CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_RIGHT_STICK_UP, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
        CNTLR_NONE, CNTLR_NONE, CNTLR_RIGHT_STICK_LEFT, CNTLR_RIGHT_STICK_DOWN, CNTLR_RIGHT_STICK_RIGHT, CNTLR_Y, CNTLR_NONE,
        CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
        CNTLR_RIGHT_STICK_BUTTON, CNTLR_LEFT_TRIGGER, CNTLR_RIGHT_TRIGGER, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
    };
#endif


void kb_init_sync() {
    kb_col_num = 0;
    gpio_bit_set(KEY_COL_GPIO_PORT[kb_col_num], KEY_COL_GPIO_PIN[kb_col_num]);
    delay_1ms(1);
}

void kb_init() {
    #ifdef SOLO
        KB_CTL |= KB_CTL_MASTER | KB_CTL_KEYBOARD | KB_CTL_USB_KB;
        kb_usb_buf = kb_buf_master;
    #else
        #if KB_SIDE == LEFT
            KB_CTL |= KB_CTL_MASTER | KB_CTL_KEYBOARD | KB_CTL_USART | KB_CTL_USB_KB;
//            KB_CTL |= KB_CTL_MASTER | KB_CTL_KEYBOARD | KB_CTL_USART | KB_CTL_USB_CDC;
        #else
            KB_CTL |= KB_CTL_KEYBOARD | KB_CTL_USART;
        #endif

        if ((KB_CTL & (KB_CTL_MASTER | KB_CTL_USART)) != 0) {
//        if ((KB_CTL & KB_CTL_MASTER) && (KB_CTL & KB_CTL_USART)) {
            kb_usb_buf = kb_buf_mix;
        } else {
            kb_usb_buf = kb_buf_master;
        }
    #endif
}


void kb_remap(uint8_t lyr) {
    kb_layout = kb_lyrs[lyr];
}

void kb_fn_handler() {
    static uint8_t kb_fn = 0;
    bool key_status = kb_key_state[kb_row_num][kb_col_num];
    if (kb_layout[kb_row_num][kb_col_num] == KEY_FN) {
        if (KB_PRESS == key_status) {
            KB_CTL |= KB_CTL_FN;
            KB_CTL &= ~KB_CTL_USB;
            KB_FLAG &= ~KB_FLAG_COUNT;
        } else {
            KB_CTL &= ~KB_CTL_FN;
            KB_CTL |= KB_CTL_USB;
            KB_FLAG &= ~KB_FLAG_COUNT;
        }
    } else {
        if (kb_row_num == 0) {
            if (key_status == KB_PRESS) {
                switch (kb_layout[kb_row_num][kb_col_num]) {
                    case KEY_F1:
                    case KEY_F6:
                        kb_fn = kb_fn_lyr;
                        break;
                    case KEY_F2:
                    case KEY_F7:
                        kb_fn = kb_fn_led;
                        break;
                    case KEY_F3:
                    case KEY_F8:
                        kb_fn = kb_fn_uart;
                        break;
                    case KEY_F4:
                    case KEY_F9:
                        kb_fn = kb_fn_ms;
                        break;
                    case KEY_F5:
                    case KEY_F10:
                        kb_fn = kb_fn_usb;
                        break;
                    default:
                        break;
                }
            } else {
                kb_fn = kb_fn_none;
            }


        } else if ((kb_row_num == 1) && (key_status == KB_PRESS)) {
            switch (kb_layout[kb_row_num][kb_col_num]) {
                case KEY_1_EXCLAMATION_MARK:
                case KEY_6_CARET:
                    switch (kb_fn) {
                        case kb_fn_lyr:
                            kb_remap(0);
                            break;
                        case kb_fn_led:
                            leds_off();
                            break;
                        case kb_fn_uart:
                            KB_CTL &= ~KB_CTL_USART;
                            kb_usb_buf = kb_buf_master;
                            break;
                        case kb_fn_ms:
                            KB_CTL &= ~KB_CTL_MASTER;
                            break;
                        case kb_fn_usb:
                            KB_CTL &= ~KB_CTL_USB;
                            break;
                        default:
                            break;
                    }
                    break;

                case KEY_2_AT:
                case KEY_7_AMPERSAND:
                    switch (kb_fn) {
                        case kb_fn_lyr:
                            kb_remap(1);
                            break;
                        case kb_fn_led:
                            leds_on();
                            break;
                        case kb_fn_uart:
                            KB_CTL |= KB_CTL_USART;
                            kb_usb_buf = kb_buf_mix;
                            break;
                        case kb_fn_ms:
                            KB_CTL |= KB_CTL_MASTER;
                            break;
                        case kb_fn_usb:
                            KB_CTL |= KB_CTL_USB;
                            break;
                        default:
                            break;
                    }
                    break;

                case KEY_3_NUMBER_SIGN:
                case KEY_8_ASTERISK:
                    switch (kb_fn) {
                        case kb_fn_lyr:
                            kb_remap(2);
                            break;
                        case kb_fn_led:
                            leds_on();
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}


//void kb_usb_buf_handler() {
//    kb_buf_num = kb_layout[kb_row_num][kb_col_num] / 8;
//    kb_buf_tmp = 1U << kb_layout[kb_row_num][kb_col_num] % 8;
//}

void kb_cntlr_usb_buf_handler() {
    switch (kb_cntlr_lyrs[0][kb_row_num][kb_col_num]) {
        default:
            kb_cntlr_buf_num = kb_cntlr_lyrs[0][kb_row_num][kb_col_num] / 8;
            kb_cntlr_buf_tmp = 1U << kb_cntlr_lyrs[0][kb_row_num][kb_col_num] % 8;
            break;

        case CNTLR_NONE:
            break;

        case CNTLR_LEFT_STICK_LEFT:
            kb_cntlr_buf_num = 2;
            kb_cntlr_buf_tmp = -CNTLR_STICK_VALUE;
            break;
        case CNTLR_LEFT_STICK_RIGHT:
            kb_cntlr_buf_num = 2;
            kb_cntlr_buf_tmp = CNTLR_STICK_VALUE;
            break;

        case CNTLR_LEFT_STICK_UP:
            kb_cntlr_buf_num = 3;
            kb_cntlr_buf_tmp = -CNTLR_STICK_VALUE;
            break;
        case CNTLR_LEFT_STICK_DOWN:
            kb_cntlr_buf_num = 3;
            kb_cntlr_buf_tmp = CNTLR_STICK_VALUE;
            break;

        case CNTLR_RIGHT_STICK_LEFT:
            kb_cntlr_buf_num = 4;
            kb_cntlr_buf_tmp = -CNTLR_STICK_VALUE;
            break;
        case CNTLR_RIGHT_STICK_RIGHT:
            kb_cntlr_buf_num = 4;
            kb_cntlr_buf_tmp = CNTLR_STICK_VALUE;
            break;

        case CNTLR_RIGHT_STICK_UP:
            kb_cntlr_buf_num = 5;
            kb_cntlr_buf_tmp = -CNTLR_STICK_VALUE;
            break;
        case CNTLR_RIGHT_STICK_DOWN:
            kb_cntlr_buf_num = 5;
            kb_cntlr_buf_tmp = CNTLR_STICK_VALUE;
            break;
    }
}

void kb_row_read_all() {
    kb_row_read = (uint8_t) (gpio_input_port_get(GPIOA) & 0x3F);
}

void kb_col_reset_all() {
    for (int i = 0; i < KB_COL_NUM; ++i) {
        gpio_bit_reset(KEY_COL_GPIO_PORT[i], KEY_COL_GPIO_PIN[i]);
    }
}

void kb_handler(){
    static bool signal = 0;
//    signal = (bool) gpio_input_bit_get(GPIOA, KEY_ROW_GPIO_PIN[kb_row_num]);
    signal = (bool) ((kb_row_read >> kb_row_num) & 0x1U);
    if (kb_key_state[kb_row_num][kb_col_num] != signal) {
        kb_key_state[kb_row_num][kb_col_num] = signal;
        KB_FLAG |= KB_FLAG_COUNT;

        if (KB_CTL & KB_CTL_KEYBOARD) {
//            kb_usb_buf_handler();
            uint8_t kb_buf_tmp = 0, kb_buf_num = 0;
            kb_buf_num = kb_layout[kb_row_num][kb_col_num] / 8;
            kb_buf_tmp = 1U << kb_layout[kb_row_num][kb_col_num] % 8;
            if (signal == KB_PRESS) {
                ++kb_key_count;
                kb_buf_master[kb_buf_num] |= kb_buf_tmp;
            } else {
                --kb_key_count;
                kb_buf_master[kb_buf_num] &= ~kb_buf_tmp;
            }
        }

        if (KB_CTL & KB_CTL_CNTLR) {
            if (kb_cntlr_lyrs[0][kb_row_num][kb_col_num] != CNTLR_NONE) {
                kb_cntlr_usb_buf_handler();
                if (signal == KB_PRESS) {
                    if (kb_cntlr_buf_num < 2) {
                        kb_buf_cntrl[kb_cntlr_buf_num] |= kb_cntlr_buf_tmp;
                    } else {
                        kb_buf_cntrl[kb_cntlr_buf_num] = kb_cntlr_buf_tmp;
                    }
                } else {
                    if (kb_cntlr_buf_num < 2) {
                        kb_buf_cntrl[kb_cntlr_buf_num] &= ~kb_cntlr_buf_tmp;
                    } else {
                        kb_buf_cntrl[kb_cntlr_buf_num] = 0;
                    }
                }

            }
        }

        if ((kb_layout[kb_row_num][kb_col_num] == KEY_FN) || (KB_CTL & KB_CTL_FN)) {
            kb_fn_handler();
        }
    }
}

void kb_polling() {
    kb_row_read_all();
    for (kb_row_num = 0; kb_row_num < KB_ROW_NUM; ++kb_row_num) {
        kb_handler();
    }
}

void kb_usb_send() {
    if (KB_CTL & KB_CTL_USB) {
        if (KB_CTL & KB_CTL_KEYBOARD) {
            if (KB_CTL & KB_CTL_REPEAT) {
                if (kb_key_count != 0) {
                    if (!(KB_FLAG & KB_FLAG_REPEAT) && (hid_kb_check_send(&usb_device) == USBD_OK)) {
                        KB_FLAG |= KB_FLAG_REPEAT;
                        hid_kb_report_send(&usb_device, kb_usb_buf/*, HID_KB_SEND_SIZE*/);
                    } else {
                        KB_FLAG &= ~KB_FLAG_REPEAT;
                        hid_kb_report_send(&usb_device, kb_buf_empty/*, HID_KB_SEND_SIZE*/);
                    }
                } else {
                    if ((KB_FLAG & KB_FLAG_COUNT) && (hid_kb_check_send(&usb_device) == USBD_OK)) {
                        if (!(KB_FLAG & KB_FLAG_REPEAT)) {
                        } else {
                            KB_FLAG &= ~KB_FLAG_REPEAT;
                        }
                        KB_FLAG &= ~KB_FLAG_COUNT;
                        hid_kb_report_send(&usb_device, kb_usb_buf/*, HID_KB_SEND_SIZE*/);
                    }
                }
            } else {
                if ((KB_FLAG & KB_FLAG_COUNT) && (hid_kb_check_send(&usb_device) == USBD_OK)) {
                    KB_FLAG &= ~KB_FLAG_COUNT;
                    hid_kb_report_send(&usb_device, kb_usb_buf/*, HID_KB_SEND_SIZE*/);
                }
            }
        }

        if (KB_CTL & KB_CTL_CNTLR) {
            hid_cntlr_report_send(&usb_device, kb_buf_cntrl, HID_CNTLR_BUF_SIZE);
        }

        if (KB_CTL & KB_CTL_MOUSE) {
            hid_mouse_report_send(&usb_device, kb_buf_mouse, HID_MICE_BUF_SIZE);
        }
    }
}

void kb_usb_receive() {
    hid_kb_report_receive (&usb_device);
    if (hid_kb_check_recev(&usb_device) == USBD_OK) {
        switch (kb_buf_recev[0]){
            case LED_NONE:
//            leds_off();
//            led_color_set(pwm, 0x00U, led_sys[0][r]);
//            led_color_set(pwm, 0x20U, led_sys[0][g]);
//            LED_FLAG |= LED_FLAG_CHANGE;
//            leds_dma();
                break;

            case 1U << LED_CAPS_LOCK:
//            leds_on();
//            led_color_set(pwm, 0x20U, led_sys[0][r]);
//            led_color_set(pwm, 0x00U, led_sys[0][g]);
                LED_FLAG |= LED_FLAG_CHANGE;
//            leds_dma();
                break;

            case 1U << LED_SHIFT:
//            led_color_set(pwm, 0x20U, led_sys[0][b]);
//            led_color_set(pwm, 0x00U, led_sys[0][g]);
                LED_FLAG |= LED_FLAG_CHANGE;
//            leds_dma();
                break;

            default:
                break;
        }

        switch (kb_buf_recev[1]) {
            case LED_OFF:
//            KB_CTL &= ~KB_CTL_LED;
//            led_effect = 0;
                led_effect = LED_NO;
                leds_off();
                break;

            case LED_ON:
//            KB_CTL |= KB_CTL_LED;
                leds_on();
                break;

            case LED_SPECTRUM:
//            leds_spectrum();
                led_effect = LED_SPECTRUM;
                break;

            case LED_NO:
                led_effect = LED_NO;
                break;

            default:
                break;
        }

        switch (kb_buf_recev[2]) {
            case KB_LYR_0:
                kb_remap(0);
                break;

            case KB_LYR_1:
                kb_remap(1);
                break;

            case KB_LYR_2:
                kb_remap(2);
                break;

            case KB_LYR_3:
                kb_remap(3);
                break;

            case KB_CTL_KB_OFF:
                KB_CTL &= ~KB_CTL_KEYBOARD;
                break;

            case KB_CTL_KB_ON:
                KB_CTL |= KB_CTL_KEYBOARD;
                break;

            case KB_CTL_REPEAT_OFF:
                KB_CTL &= ~KB_CTL_REPEAT;
//            kb_repeat_flag = 0;
//            KB_FLAG &= KB_FLAG_REPEAT;
                break;

            case KB_CTL_REPEAT_ON:
                KB_CTL |= KB_CTL_REPEAT;
//            kb_repeat_flag = 0;
//            KB_FLAG |= KB_FLAG_REPEAT;
                break;

            default:
                //kb_remap(hid->data_out[2]);
                break;
        }
    }
}

void kb_usart_polling() {
    if (KB_CTL & KB_CTL_USART) {
        if (KB_CTL & KB_CTL_MASTER) {

//            if (!flag) {
//                dma_config(5);
//                dma_channel_enable(DMA0, DMA_CH2);
//                flag = 1;
//            }

            if (dma_flag_get(DMA0, DMA_CH2, DMA_FLAG_FTF)/* && (flag == 1)*/) {
//                dma_flag_clear(DMA0, DMA_CH2, DMA_FLAG_FTF);
//                dma_channel_disable(DMA0, DMA_CH2);

                // delete noise
                if (dma_transfer_number_get(DMA0, DMA_CH2) != 0) {
                    for (uint8_t i = 0; i < HID_KB_SEND_SIZE; ++i) {
                        kb_buf_slave[i] = 0;
                    }
                } else {
//                    flag = 0;

                    // add condition
                    KB_FLAG |= KB_FLAG_COUNT;
                }

//                dma_channel_disable(DMA0, DMA_CH2);
                dma_config(5);
//                dma_channel_enable(DMA0, DMA_CH2);

            }

            for (uint8_t i = 0; i < HID_KB_SEND_SIZE; ++i) {
                kb_buf_mix[i] = kb_buf_master[i] | kb_buf_slave[i];
            }

        } else {
            static bool flag = 0;
            if ((KB_FLAG & KB_FLAG_COUNT) && !flag) {
                KB_FLAG &= ~KB_FLAG_COUNT;
                dma_config(4);
//                dma_channel_enable(DMA0, DMA_CH1);
                flag = 1;
            }

            if (dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF)) {
                dma_flag_clear(DMA0, DMA_CH1, DMA_FLAG_FTF);
//                dma_channel_disable(DMA0, DMA_CH1);
                flag = 0;
            }
        }
    }
}

void kb_uart_init() {
    if (KB_CTL & KB_CTL_USART) {
        dma_config(4);
        dma_config(5);
        usart_config();
    }
}

void kb_it() {
    switch (kb_col_num) {
        default:
            kb_polling();

            gpio_bit_reset(KEY_COL_GPIO_PORT[kb_col_num], KEY_COL_GPIO_PIN[kb_col_num]);
            ++kb_col_num;
            gpio_bit_set(KEY_COL_GPIO_PORT[kb_col_num], KEY_COL_GPIO_PIN[kb_col_num]);
            break;

        case 6:
            kb_polling();

            gpio_bit_reset(KEY_COL_GPIO_PORT[kb_col_num], KEY_COL_GPIO_PIN[kb_col_num]);
            ++kb_col_num;
            break;

        case 7:
            kb_col_num = 0;
            gpio_bit_set(KEY_COL_GPIO_PORT[kb_col_num], KEY_COL_GPIO_PIN[kb_col_num]);

            kb_usart_polling();
            kb_usb_send();
//            kb_usb_receive();
//            kb_usart_debug();
            led_polling();
            break;
    }

}



void kb_usart_debug() {
    #if KB_SIDE == LEFT
    // rx
//        dma_config(5);
//        dma_channel_enable(DMA0, DMA_CH2);
//        usart_polling();
    if (KB_CTL & KB_CTL_USB) {
//        cdc_acm_data_send_2(&usb_device, kb_usb_buf, 12);
        cdc_acm_data_send_2(&usb_device, kb_buf_slave, HID_KB_SEND_SIZE);
    }
        //    usart_flag_clear(USART2, USART_FLAG_RBNE);
//        for (int i = 0; i < HID_KB_SEND_SIZE; ++i) {
//            kb_buf_slave[i] = 0x01;
//        }
//        for (int i = 0; i < HID_KB_SEND_SIZE; ++i) {
//            kb_buf_mix[i] = 0x02;
//        }
    #else
    // tx
        static uint8_t j = 0;
        for (int i = 0; i < HID_KB_SEND_SIZE; ++i) {
            if (j == 0xff) {
                j = 0;
            }
            kb_buf_master[i] = ++j;
        }
        dma_config(4);
        dma_channel_enable(DMA0, DMA_CH1);
    #endif
}

#ifdef KB_CTL_ADC
void adc_it() {
    static uint16_t count = 0;

    adc_char[count] = adc_value[0];

    if (count == 9) {
        count = 0;

//    cdc_example();

//        uint16_t a[64] = {11, 12, 13, 14};
//        cdc_acm_data_send_2(&usb_device, (uint8_t*) a, 8);

//        uint8_t a[64] = {0x11, 0x12, 0x13, 0x14};
//        cdc_acm_data_send_2(&usb_device, a, 4);

//        uint8_t a[64] = {0x61, 0x62, 0x63, 0x64};
//        cdc_acm_data_send_2(&usb_device, a, 4);

//        uint8_t a[64] = {'a', 'b', 'c', 'd'};
//        cdc_acm_data_send_2(&usb_device, a, 1);

//        uint8_t a[64] = "a";
//        cdc_acm_data_send_2(&usb_device, a, 2);

//        cdc_acm_data_send_2 (&usb_device, (uint8_t *) adc_value, 12);


//        adc_main(count);
//        cdc_acm_data_send_2 (&usb_device, (uint8_t *) adc_value, 2);
        if (0 == cdc_acm_check_send(&usb_device)) {
            cdc_acm_data_send_2 (&usb_device, (uint8_t *) adc_char, 10 * 2);
        }
//        cdc_acm_data_send_2 (&usb_device, (uint8_t *) &count, 2);
//        cdc_acm_data_send_2 (&usb_device, kb_buf_slave, 12);
    } else {
        ++count;
    }
}
#endif