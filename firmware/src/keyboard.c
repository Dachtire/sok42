#include "keyboard.h"
#include "usbd_hid_keyboard.h"
#include "usbd_vendor.h"


const uint16_t KB_ROW_GPIO_PIN[KB_ROW_NUM] = {
        KB_ROW_0_GPIO_PIN, KB_ROW_1_GPIO_PIN, KB_ROW_2_GPIO_PIN,
        KB_ROW_3_GPIO_PIN, KB_ROW_4_GPIO_PIN, KB_ROW_5_GPIO_PIN};
const uint16_t KB_COL_GPIO_PIN[KB_COL_NUM] = {
        KB_COL_0_GPIO_PIN, KB_COL_1_GPIO_PIN, KB_COL_2_GPIO_PIN,KB_COL_3_GPIO_PIN,
        KB_COL_4_GPIO_PIN, KB_COL_5_GPIO_PIN,KB_COL_6_GPIO_PIN};
GPIO_TypeDef* const KB_COL_GPIO_PORT[KB_COL_NUM] = {
        KB_COL_0_GPIO_PORT, KB_COL_1_GPIO_PORT, KB_COL_2_GPIO_PORT,KB_COL_3_GPIO_PORT,
        KB_COL_4_GPIO_PORT, KB_COL_5_GPIO_PORT, KB_COL_6_GPIO_PORT};

uint8_t kb_usbhd_buf[USBD_KB_SEND_SIZE] = {}, kb_usbd_buf[USBD_KB_SEND_SIZE] = {}, kb_buf_device[USBD_KB_SEND_SIZE] = {}, kb_buf_empty[USBD_KB_SEND_SIZE] = {};
uint8_t kb_buf_cntrl[USBD_CNTLR_BUF_SIZE] = {};
uint8_t kb_buf_mouse[USBD_MICE_BUF_SIZE] = {};

uint8_t kb_buf_recev[USBD_KB_RECEV_SIZE];

uint8_t kb_col_num = 0, kb_row_num = 0;
uint8_t (*kb_layout)[KB_COL_NUM] = kb_lyrs[0];
bool kb_key_state[KB_ROW_NUM][KB_COL_NUM] = {};
uint32_t kb_flag;
uint8_t kb_key_count = 0;
uint8_t kb_cntlr_buf_num = 0, kb_cntlr_buf_tmp = 0;

uint32_t kb_ctl = 0;
uint8_t *kb_usb_buf = 0;

uint8_t kb_row_read = 0;
uint16_t kb_adc_value[KB_ADC_SIZE] = {}, kb_adc_buf1[KB_ADC_SIZE] = {};


#if KB_SIDE == KB_SIDE_LEFT
    static uint8_t kb_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM] = {
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

    static uint8_t kb_cntlr_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM] = {
        CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
        CNTLR_BACK, CNTLR_LEFT_STICK_BUTTON, CNTLR_LEFT_STICK_LEFT, CNTLR_LEFT_STICK_RIGHT, CNTLR_LEFT_STICK_UP, CNTLR_LEFT_STICK_DOWN, CNTLR_NONE,
        CNTLR_START, CNTLR_RIGHT_STICK_BUTTON, CNTLR_RIGHT_STICK_LEFT, CNTLR_RIGHT_STICK_RIGHT, CNTLR_RIGHT_STICK_UP, CNTLR_RIGHT_STICK_DOWN, CNTLR_NONE,
        CNTLR_RIGHT_BUMPER, CNTLR_RIGHT_TRIGGER, CNTLR_A, CNTLR_B, CNTLR_X, CNTLR_Y, CNTLR_NONE,
        CNTLR_LEFT_BUMPER, CNTLR_LEFT_TRIGGER, CNTLR_DPAD_LEFT, CNTLR_DPAD_RIGHT, CNTLR_DPAD_UP, CNTLR_DPAD_DOWN, CNTLR_NONE,
        CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE, CNTLR_NONE,
    };
#else
    static uint8_t kb_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM] = {
        KEY_F12, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11,
        KEY_OBRACKET_AND_OBRACE, KEY_6_CARET, KEY_7_AMPERSAND, KEY_8_ASTERISK, KEY_9_OPARENTHESIS, KEY_0_CPARENTHESIS, KEY_CBRACKET_AND_CBRACE,
        KEY_EQUAL_PLUS, KEY_F, KEY_G, KEY_C, KEY_R, KEY_L, KEY_BACKSLASH_VERTICAL_BAR,
        KEY_MINUS_UNDERSCORE, KEY_D, KEY_H, KEY_T, KEY_N, KEY_S, KEY_BACKSPACE,
        KEY_SLASH_QUESTION, KEY_B, KEY_M, KEY_W, KEY_V, KEY_Z, KEY_RIGHT_SHIFT,
        KEY_RIGHT_ALT, KEY_SPACEBAR, KEY_RIGHT_CONTROL, KEY_RIGHT_GUI, KEY_INSERT, KEY_PRINTSCREEN, KEY_FN,

        KEY_F12, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11,
        KEY_OBRACKET_AND_OBRACE, KEY_6_CARET, KEY_7_AMPERSAND, KEY_8_ASTERISK, KEY_9_OPARENTHESIS, KEY_0_CPARENTHESIS, KEY_CBRACKET_AND_CBRACE,
        KEY_EQUAL_PLUS, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_BACKSLASH_VERTICAL_BAR,
        KEY_MINUS_UNDERSCORE, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON_COLON, KEY_BACKSPACE,
        KEY_SLASH_QUESTION, KEY_N, KEY_M, KEY_COMMA_AND_LESS, KEY_DOT_GREATER, KEY_SINGLE_AND_DOUBLE_QUOTE, KEY_RIGHT_SHIFT,
        KEY_RIGHT_ALT, KEY_SPACEBAR, KEY_RIGHT_CONTROL, KEY_DELETE, KEY_RIGHT_GUI, KEY_INSERT, KEY_FN,

   };

    static uint8_t kb_cntlr_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM] = {
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
    GPIO_SetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
    Delay_Us(20);
}

void kb_init() {
    kb_device = KB_DEVICE_KEYBORAD;
//    kb_device = KB_DEVICE_VENDOR;
//    kb_device = KB_DEVICE_KEYBOARD_ADC_DIFF;
//    kb_device = KB_DEVICE_KEYBOARD_ADC_TRIGGER;

    if ((kb_ctl & KB_CTL_HOST) != 0) {
        kb_usb_buf = kb_usbhd_buf;
    } else {
        kb_usb_buf = kb_usbd_buf;
    }
//    printf("kb_ctl:%02lx\n", kb_ctl);
}

void kb_enable_usbd() {
    kb_ctl |= KB_CTL_USBD;
//    printf("kb_ctl:%02lx\n", kb_ctl);
}

void kb_it() {
    switch (kb_col_num) {
        default:
            kb_polling();

            GPIO_ResetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
            ++kb_col_num;
            GPIO_SetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
            break;

        case 6:
            kb_polling();

            GPIO_ResetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
            ++kb_col_num;

            usbh_epin_time();
            USBH_MainDeal();
            break;

        case 7:
            kb_col_num = 0;
            GPIO_SetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);

            //            kb_usart_polling();
            kb_usbhd_combine();
            kb_usb_send();
//            kb_usbd_hid_test();
            //            kb_usb_receive();
            //            kb_usart_debug();
            //            led_polling();
//            kb_usbd_test();
            break;
    }

}

void kb_remap(uint8_t lyr) {
    kb_layout = kb_lyrs[lyr];
}

void kb_fn_handler() {
//    static uint8_t kb_fn = 0;
//    bool key_status = kb_key_state[kb_row_num][kb_col_num];
//    if (kb_layout[kb_row_num][kb_col_num] == KEY_FN) {
//        if (KB_PRESS == key_status) {
//            kb_ctl |= KB_FLAG_FN;
//            kb_ctl &= ~KB_CTL_USBD;
//            kb_flag &= ~KB_FLAG_COUNT;
//        } else {
//            kb_ctl &= ~KB_FLAG_FN;
//            kb_ctl |= KB_CTL_USBD;
//            kb_flag &= ~KB_FLAG_COUNT;
//        }
//    } else {
//        if (key_status == KB_PRESS) {
        switch (kb_layout[kb_row_num][kb_col_num]) {
            default:
                break;

            case KEY_F1:
            case KEY_F6: {
                static uint8_t lyr = 0;
                if (lyr == 0) {
                    lyr = 1;
                    kb_remap(lyr);
                } else {
                    lyr = 0;
                    kb_remap(lyr);
                }
//                kb_flag &= ~KB_FLAG_FN;
                break;
            }

            case KEY_F2:
            case KEY_F7:
                //                    kb_fn = kb_fn_lyr;
                if (kb_repeat == KB_REPEAT_SLOW) {
                    kb_repeat = KB_REPEAT_NONE;
                } else {
                    kb_repeat = KB_REPEAT_SLOW;
                }
//                kb_flag &= ~KB_FLAG_FN;
                break;

            case KEY_F4:
            case KEY_F9:
//                    kb_fn = kb_fn_lyr;
                if (kb_repeat == KB_REPEAT_FAST) {
//                    kb_ctl &= ~KB_CTL_REPEAT;
kb_repeat = KB_REPEAT_NONE;
                } else {
                    kb_repeat = KB_REPEAT_FAST;
                }
//                kb_flag &= ~KB_FLAG_FN;
                break;

            case KEY_F5:
            case KEY_F10:
//                    kb_fn = kb_fn_usb;
//                if (kb_ctl & KB_CTL_USBD) {
//                    kb_ctl &= ~KB_CTL_USBD;
//                } else {
//                    kb_ctl |= KB_CTL_USBD;
//                }
                break;
       }
//        }
//        if (kb_row_num == 0) {
//            if (key_status == KB_PRESS) {
//                switch (kb_layout[kb_row_num][kb_col_num]) {
//                    case KEY_F1:
//                    case KEY_F6:
//                        kb_fn = kb_fn_lyr;
//                        break;
//                    case KEY_F2:
//                    case KEY_F7:
////                        kb_fn = kb_fn_led;
//                        kb_fn = kb_fn_ctl;
//                        break;
//                    case KEY_F3:
//                    case KEY_F8:
//                        kb_fn = kb_fn_uart;
//                        break;
//                    case KEY_F4:
//                    case KEY_F9:
//                        kb_fn = kb_fn_ms;
//                        break;
//                    case KEY_F5:
//                    case KEY_F10:
//                        kb_fn = kb_fn_usb;
//                        break;
//                    default:
//                        break;
//                }
//            } else {
//                kb_fn = kb_fn_none;
//            }
//        } else if ((kb_row_num == 1) && (key_status == KB_PRESS)) {
//            switch (kb_layout[kb_row_num][kb_col_num]) {
//                case KEY_1_EXCLAMATION_MARK:
//                case KEY_6_CARET:
//                    switch (kb_fn) {
//                        case kb_fn_lyr:
//                            kb_remap(0);
//                            break;
//                        case kb_fn_led:
////                            leds_off();
//                            break;
//                        case kb_fn_uart:
//                            kb_ctl &= ~KB_CTL_USART;
//                            kb_usb_buf = kb_usbd_buf;
//                            break;
//                        case kb_fn_ms:
//                            kb_ctl &= ~KB_CTL_HOST;
//                            break;
//                        case kb_fn_usb:
//                            kb_ctl &= ~KB_CTL_USBD;
//                            break;
//                        default:
//                            break;
//                    }
//                    break;
//
//                case KEY_2_AT:
//                case KEY_7_AMPERSAND:
//                    switch (kb_fn) {
//                        case kb_fn_lyr:
//                            kb_remap(1);
//                            break;
//                        case kb_fn_ctl:
//                        case kb_fn_led:
////                            leds_on();
//                            break;
//                        case kb_fn_uart:
//                            kb_ctl |= KB_CTL_USART;
//                            kb_usb_buf = kb_usbhd_buf;
//                            break;
//                        case kb_fn_ms:
//                            kb_ctl |= KB_CTL_HOST;
//                            break;
//                        case kb_fn_usb:
//                            kb_ctl |= KB_CTL_USBD;
//                            break;
//                        default:
//                            break;
//                    }
//                    break;
//
//                case KEY_3_NUMBER_SIGN:
//                case KEY_8_ASTERISK:
//                    switch (kb_fn) {
//                        case kb_fn_lyr:
//                            kb_remap(2);
//                            break;
//                        case kb_fn_led:
////                            leds_on();
//                            break;
//                        default:
//                            break;
//                    }
//                    break;
//                default:
//                    break;
//            }
//        }
//    }
}

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
    kb_row_read = (uint8_t) (GPIO_ReadInputData(GPIOA) & 0x3F);
    //    if (kb_row_read != 0) {
    //        printf("r[][%d]:%d\n", kb_col_num, kb_row_read);
    //   }
}

void kb_col_reset_all() {
    for (int i = 0; i < KB_COL_NUM; ++i) {
        GPIO_ResetBits(KB_COL_GPIO_PORT[i], KB_COL_GPIO_PIN[i]);
    }
}

void kb_handler(){
    static bool signal = 0;
//    signal = (bool) GPIO_ReadInputDataBit(GPIOA, KB_ROW_GPIO_PIN[kb_row_num]);
    signal = (bool) ((kb_row_read >> kb_row_num) & 0x1);
    if (kb_key_state[kb_row_num][kb_col_num] != signal) {
        kb_key_state[kb_row_num][kb_col_num] = signal;
        kb_flag |= KB_FLAG_COUNT;

        if (kb_layout[kb_row_num][kb_col_num] == KEY_FN) {
            if (signal == KB_PRESS) {
                kb_flag |= KB_FLAG_FN;
//                kb_ctl &= ~KB_CTL_USBD;
                kb_flag &= ~KB_FLAG_COUNT;
//                kb_remap(1);
            } else {
                kb_flag &= ~KB_FLAG_FN;
//                kb_ctl |= KB_CTL_USBD;
                kb_flag &= ~KB_FLAG_COUNT;
//                kb_remap(0);
            }
        } else {
            if (kb_flag & KB_FLAG_FN) {
                if (signal == KB_PRESS) {
                    kb_fn_handler();
                }
                kb_flag &= ~KB_FLAG_COUNT;
            }
        }

        switch (kb_device) {
            default:
            case KB_DEVICE_KEYBORAD: {
                uint8_t kb_buf_tmp = 0, kb_buf_num = 0;
                kb_buf_num = kb_layout[kb_row_num][kb_col_num] / 8;
                kb_buf_tmp = 1U << kb_layout[kb_row_num][kb_col_num] % 8;
                if (signal == KB_PRESS) {
                    ++kb_key_count;
                    kb_usbd_buf[kb_buf_num] |= kb_buf_tmp;
                } else {
                    --kb_key_count;
                    kb_usbd_buf[kb_buf_num] &= ~kb_buf_tmp;
                }
                break;
            }

            case KB_DEVICE_CNTLR:
//                if (kb_ctl & KB_CTL_CNTLR) {
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
//                }
                break;
        }
    }
}

void kb_polling() {
    switch (kb_device) {
        default:
        case KB_DEVICE_KEYBORAD:
            kb_row_read_all();
            break;

        case KB_DEVICE_KEYBOARD_ADC_DIFF:
            kb_row_adc_diff();
            break;

        case KB_DEVICE_KEYBOARD_ADC_TRIGGER:
            kb_row_adc_trigger();
            break;
    }
    for (kb_row_num = 0; kb_row_num < KB_ROW_NUM; ++kb_row_num) {
        kb_handler();
    }
}

void kb_usb_send() {
    if (kb_ctl & KB_CTL_USBD) {
        switch (kb_device) {
            default:
            case KB_DEVICE_KEYBORAD:
                switch (kb_repeat) {
                    default:
                    case KB_REPEAT_NONE:
                        if ((kb_flag & KB_FLAG_COUNT) && (usbd_kb_check_send() == USB_SUCCESS)) {
                            kb_flag &= ~KB_FLAG_COUNT;
                            usbd_kb_report_send(kb_usb_buf);
                        }
                        break;

                    case KB_REPEAT_FAST:
                        if (kb_key_count != 0) {
                            if (!(kb_flag & KB_FLAG_REPEAT) && (usbd_kb_check_send() == USB_SUCCESS)) {
                                kb_flag |= KB_FLAG_REPEAT;
                                usbd_kb_report_send(kb_usb_buf);
                            } else {
                                kb_flag &= ~KB_FLAG_REPEAT;
                                usbd_kb_report_send(kb_buf_empty);
                            }
                        } else {
                            if ((kb_flag & KB_FLAG_COUNT) && (usbd_kb_check_send() == USB_SUCCESS)) {
                                if (!(kb_flag & KB_FLAG_REPEAT)) {
                                } else {
                                    kb_flag &= ~KB_FLAG_REPEAT;
                                }
                                kb_flag &= ~KB_FLAG_COUNT;
                                usbd_kb_report_send(kb_usb_buf);
                            }
                        }
                        break;

                    case KB_REPEAT_SLOW: {
                        static uint16_t kb_repeat_delay = 0, kb_repeat_rate = 0;
                        static bool kb_repeat_first = 0;
                        if (kb_key_count != 0) {
                            if ((kb_repeat_delay == KB_REPEAT_SLOW_DELAY && kb_repeat_rate == KB_REPEAT_SLOW_RATE) || (kb_repeat_first == 0)) {
                                if (!(kb_flag & KB_FLAG_REPEAT) && (usbd_kb_check_send() == USB_SUCCESS)) {
                                    kb_flag |= KB_FLAG_REPEAT;
                                    usbd_kb_report_send(kb_usb_buf);
    //                                kb_repeat_delay = 0;
                                    kb_repeat_rate = 0;
                                    kb_repeat_first = 1;
                                } else {
    //                                if (kb_repeat_rate == KB_REPEAT_SLOW_RATE) {
                                    kb_flag &= ~KB_FLAG_REPEAT;
                                    usbd_kb_report_send(kb_buf_empty);
    //                                    kb_repeat_rate = 0;
    //                                }
                                }
                            } else {
                                if (kb_repeat_delay != KB_REPEAT_SLOW_DELAY) {
                                    ++kb_repeat_delay;
//                                    kb_repeat_rate = KB_REPEAT_SLOW_RATE;
                                } else {
                                    if (kb_repeat_rate != KB_REPEAT_SLOW_RATE)
                                        ++kb_repeat_rate;
                                }
                            }
                        } else {
                            if ((kb_flag & KB_FLAG_COUNT) && (usbd_kb_check_send() == USB_SUCCESS)) {
                                kb_flag &= ~KB_FLAG_REPEAT;
                                kb_flag &= ~KB_FLAG_COUNT;
                                usbd_kb_report_send(kb_usb_buf);
                                kb_repeat_first = 0;
                                kb_repeat_delay = 0;
                                kb_repeat_rate = 0;
                            }
                        }
                        break;
                    }
                }
                break;

            case KB_DEVICE_CNTLR:
                //            hid_cntlr_report_send(kb_buf_cntrl, USBD_CNTLR_BUF_SIZE);

                break;

            case KB_DEVICE_MOUSE:
                //            hid_mouse_report_send(kb_buf_mouse, USBD_MICE_BUF_SIZE);

                break;
        }

//        if (kb_ctl & KB_CTL_CNTLR) {
//            hid_cntlr_report_send(kb_buf_cntrl, USBD_CNTLR_BUF_SIZE);
//        }

//        if (kb_ctl & KB_CTL_MOUSE) {
//            hid_mouse_report_send(kb_buf_mouse, USBD_MICE_BUF_SIZE);
//        }
    }
}

void kb_usb_receive() {
    usbd_kb_report_receive();
    if (usbd_kb_check_recev() == USB_SUCCESS) {
/*        switch (kb_buf_recev[0]){
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
//            kb_ctl &= ~KB_CTL_LED;
//            led_effect = 0;
                led_effect = LED_NO;
                leds_off();
                break;

            case LED_ON:
//            kb_ctl |= KB_CTL_LED;
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
        }*/

        switch (kb_buf_recev[2]) {
            case KB_CTL_LYR_0:
                kb_remap(0);
                break;

            case KB_CTL_LYR_1:
                kb_remap(1);
                break;

            case KB_CTL_LYR_2:
                kb_remap(2);
                break;

            case KB_CTL_LYR_3:
                kb_remap(3);
                break;

            case KB_CTL_KB_OFF:
//                kb_ctl &= ~KB_CTL_KEYBOARD;
                break;

            case KB_CTL_KB_ON:
//                kb_ctl |= KB_CTL_KEYBOARD;
                break;

            case KB_CTL_REPEAT_OFF:
//                kb_ctl &= ~KB_CTL_REPEAT;
//            kb_repeat_flag = 0;
//            kb_flag &= KB_FLAG_REPEAT;
                break;

            case KB_CTL_REPEAT_ON:
//                kb_ctl |= KB_CTL_REPEAT;
//            kb_repeat_flag = 0;
//            kb_flag |= KB_FLAG_REPEAT;
                break;

            default:
                //kb_remap(hid->data_out[2]);
                break;
        }
    }
}

void kb_usbhd_combine() {
    if ((kb_flag & KB_FLAG_COUNT) && (kb_ctl & KB_CTL_HOST)) {
//        printf("usbhd: ");
        for (uint8_t i = 0; i < USBD_KB_SEND_SIZE; ++i) {
            kb_usbhd_buf[i] = kb_usbd_buf[i] | kb_usbh_buf[i];
//            printf("%02x ", kb_usb_buf[i]);
        }
//        printf("\n");
    }
}

/*void kb_usart_polling() {
    if (kb_ctl & KB_CTL_USART) {
        if (kb_ctl & KB_CTL_HOST) {

//            if (!flag) {
//                dma_config(5);
//                dma_channel_enable(DMA0, DMA_CH2);
//                flag = 1;
//            }

            if (dma_flag_get(DMA0, DMA_CH2, DMA_FLAG_FTF)*//* && (flag == 1)*//*) {
//                dma_flag_clear(DMA0, DMA_CH2, DMA_FLAG_FTF);
//                dma_channel_disable(DMA0, DMA_CH2);

                // delete noise
                if (dma_transfer_number_get(DMA0, DMA_CH2) != 0) {
                    for (uint8_t i = 0; i < USBD_KB_SEND_SIZE; ++i) {
                        kb_buf_device[i] = 0;
                    }
                } else {
//                    flag = 0;

                    // add condition
                    kb_flag |= KB_FLAG_COUNT;
                }

//                dma_channel_disable(DMA0, DMA_CH2);
                dma_config(5);
//                dma_channel_enable(DMA0, DMA_CH2);

            }

            for (uint8_t i = 0; i < USBD_KB_SEND_SIZE; ++i) {
                kb_usbhd_buf[i] = kb_usbd_buf[i] | kb_buf_device[i];
            }

        } else {
            static bool flag = 0;
            if ((kb_flag & KB_FLAG_COUNT) && !flag) {
                kb_flag &= ~KB_FLAG_COUNT;
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
}*/

//void kb_uart_init() {
//    if (kb_ctl & KB_CTL_USART) {
//        dma_config(4);
//        dma_config(5);
//        usart_config();
//    }
//}


/*void kb_usart_debug() {
    #if KB_CTL_SIDE_LEFT == LEFT
    // rx
//        dma_config(5);
//        dma_channel_enable(DMA0, DMA_CH2);
//        usart_polling();
    if (kb_ctl & KB_CTL_USBD) {
//        cdc_acm_data_send_2(&usb_device, kb_usb_buf, 12);
        cdc_acm_data_send_2(&usb_device, kb_buf_device, USBD_KB_SEND_SIZE);
    }
        //    usart_flag_clear(USART2, USART_FLAG_RBNE);
//        for (int i = 0; i < USBD_KB_SEND_SIZE; ++i) {
//            kb_buf_device[i] = 0x01;
//        }
//        for (int i = 0; i < USBD_KB_SEND_SIZE; ++i) {
//            kb_usbhd_buf[i] = 0x02;
//        }
    #else
    // tx
        static uint8_t j = 0;
        for (int i = 0; i < USBD_KB_SEND_SIZE; ++i) {
            if (j == 0xff) {
                j = 0;
            }
            kb_usbd_buf[i] = ++j;
        }
        dma_config(4);
        dma_channel_enable(DMA0, DMA_CH1);
    #endif
}*/

void kb_adc_it() {
    static uint32_t delay = 0;
    if (delay == 100000) {
        delay = 0;
    } else {
        ++delay;
    }

//    static uint16_t count = 0;
//    static uint8_t flag = 0;
//
////    kb_adc_buf1[count] = kb_adc_value[0] + count;
//    static bool tmp = 1;
//    tmp = ~tmp;
//    GPIO_WriteBit(KB_COL_GPIO_PORT[0], KB_COL_GPIO_PIN[0], (BitAction) tmp);
//    static uint32_t j = 0;
//
//    if (j == 1000 * 5)
//    {
////        kb_adc_buf1[count] = kb_adc_value[0];
//        switch (count) {
////            case 0:
////                for (uint16_t i = 0; i < KB_ADC_SIZE; ++i) {
////                    kb_adc_value[i] = 0;
////                }
////
////                ++count;
////                break;
//
////            default:
//            case 5 - 1:
//                //            kb_usbd_test((uint8_t) (0x10f6), 2);
//                //            kb_usbd_test(count, 2);t
//                //            conv = Get_ConversionVal(kb_adc_value[0]);
//                //            kb_usbd_test(kb_adc_value, 16);1
//                //            if (kb_ctl & KB_CTL_USBD) {
//                //                usbd_vendor_send((uint8_t *) kb_adc_value, 16, ENDP1);
//                //            }
//                //            kb_usbd_test(cali, 2);
//                //            kb_usbd_test(conv, 2);
//                tmp = ~tmp;
//                GPIO_WriteBit(KB_COL_GPIO_PORT[0], KB_COL_GPIO_PIN[0], (BitAction) tmp);
//                //            GPIO_ResetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
//                //            ++kb_col_num;
//                //            GPIO_SetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
//
//                //            kb_adc_buf1[count] = count;
//                //            for (uint16_t i = 0; i < KB_ADC_SIZE; ++i) {
//                //                kb_adc_value[i] = i + KB_ADC_SIZE * j;
//                //            }
//                //            ++j;
//
//                ++count;
//                break;
//
//                //        case KB_ADC_SIZE / 2 - 2:
//                //        case KB_ADC_SIZE - 2:
//                //            ++count;
//                //
//                //            flag = !flag;
//                //            GPIO_WriteBit(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num], flag);
//                //            break;
//
//
//                //        case 8:
//                ////            kb_usbd_test(0xf6);
//                //            break;
//
//            case 10 - 1:
//                //        case KB_ADC_SIZE - 1:
//                //            kb_usbd_test(count, 2);
//                //            GPIO_ResetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
//                //            kb_col_num = 0;
//                //            GPIO_SetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
//                //            if (USBD_OK == cdc_acm_check_send(&usb_device)) {
//                //                cdc_acm_data_send_2 (&usb_device, (uint8_t *) kb_adc_buf1, KB_ADC_SIZE * 2);
//                //            }
//                //            if (kb_ctl & KB_CTL_USBD) {
//                ////                usbd_vendor_send((uint8_t *) kb_adc_buf1, KB_ADC_SIZE, ENDP1);
//                //                usbd_vendor_send((uint8_t *) kb_adc_buf1, 10, ENDP1);
//                //
//                //            }
////                DMA_Cmd(DMA1_Channel1, DISABLE);
////                ADC_SoftwareStartConvCmd(ADC1, DISABLE);
////                tmp = ~tmp;
////                GPIO_WriteBit(KB_COL_GPIO_PORT[0], KB_COL_GPIO_PIN[0], (BitAction) tmp);
//
//                if ((kb_ctl & KB_CTL_USBD) /* != 0) && (usbd_vendor_check_send() == USB_SUCCESS)*/) {
//                    //                usbd_vendor_send((uint8_t *) kb_adc_value, KB_ADC_LEN, ENDP1);
//                    usbd_vendor_send_mult((uint8_t *) kb_adc_value, KB_ADC_LEN, ENDP1);
////                    usbd_vendor_send_mult((uint8_t *) kb_adc_buf1, KB_ADC_LEN, ENDP1);
//                }
//
//                count = 0;
//                j = 0;
//                break;
//        }
//    } else {
//        ++j;
//    }

    //    switch (kb_col_num) {
//        default:
//            GPIO_ResetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
//            ++kb_col_num;
//            GPIO_SetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
//            break;
//
//        case 6:
//            GPIO_ResetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
//            ++kb_col_num;
//            break;
//
//        case 7:
//            kb_col_num = 0;
//            GPIO_SetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
//            break;
//    }
    

//    if (count == 9) {
//        count = 0;

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

//        cdc_acm_data_send_2 (&usb_device, (uint8_t *) kb_adc_value, 12);


//        adc_main(count);
//        cdc_acm_data_send_2 (&usb_device, (uint8_t *) kb_adc_value, 2);
//        if (USBD_OK == cdc_acm_check_send(&usb_device)) {
//            cdc_acm_data_send_2 (&usb_device, (uint8_t *) kb_adc_buf1, 10 * 2);
//        }
//        cdc_acm_data_send_2 (&usb_device, (uint8_t *) &count, 2);
//        cdc_acm_data_send_2 (&usb_devic e, kb_buf_device, 12);
//    } else {
//        ++count;
//    }
}

void kb_usbd_test(uint8_t data, uint16_t length) {
//    uint8_t a[12] = {0x02,};
//    uint8_t b[12] = {0x00,};
//    kb_usb_buf[0] = 0x02;
//    static uint16_t c = 0;
//    switch (c) {
//        case 125 - 1:
//            kb_usb_buf[0] = 0x02;
//            usbd_kb_report_send(kb_usb_buf);
//            ++c;
//            break;
//
//        case 250 - 1:
//            kb_usb_buf[0] = 0x00;
//            usbd_kb_report_send(kb_usb_buf);
//            c = 0;
//            break;
//
//        default:
//            ++c;
//            break;
//    }

//    uint8_t tmp = 0xf1;
    if (kb_ctl & KB_CTL_USBD) {
        usbd_vendor_send(&data, length, ENDP1);
    }
}

void kb_usbd_hid_test() {
//    Delay_Ms(500);

    static uint16_t count = 0;
    static bool flag = 0;
    if (count == 60) {
        count = 0;

        if (!flag) {
            kb_usb_buf[0] = 2;
        } else {
            kb_usb_buf[0] = 0;
        }

        flag = !flag;

        if (kb_ctl & KB_CTL_USBD) {
            usbd_kb_report_send(kb_usb_buf);
        }
    } else {
        ++count;
    }

//        uint8_t a[12] = {0x02,};
//        uint8_t b[12] = {0x00,};
//        static uint8_t c = 0;
//        if (c == 0) {
//            usbd_kb_report_send(a);
//            c = 1;
//        } else {
//            usbd_kb_report_send(b);
//            c = 0;
//        }
}

void kb_adc_time() {
    static bool min_once = 0, max_once = 0;
    static uint32_t us = 0;
    const static uint16_t max = 4096 * 0.9;
    const static uint16_t min = 4096 * 0.2;
    if (kb_adc_value[0] < min) {
        if (!min_once) {
            printf("us:%lu, min:%d\n", us, kb_adc_value[0]);
            min_once = 1;
            max_once = 0;
            us = 0;
        }
    } else if (kb_adc_value[0] <= max) {
//        printf("i:%d\n", kb_adc_value[0]);
        ++us;
    } else {
        if (!max_once) {
            printf("us:%lu, max:%d\n", us, kb_adc_value[0]);
            max_once = 1;
            min_once = 0;
            us = 0;
        }
    }

//        GPIO_ResetBits(KB_COL_GPIO_PORT[kb_col_num], KB_COL_GPIO_PIN[kb_col_num]);
}

void kb_row_adc_diff() {
    static uint16_t kb_sw_adc[KB_ROW_NUM][KB_COL_NUM];
    static const uint16_t press_step = 64, release_step = 256;

    kb_row_read = 0;
    for (int i = 0; i < KB_ROW_NUM; ++i) {
        if (kb_sw_adc[i][kb_col_num] == 0) {
            kb_sw_adc[i][kb_col_num] = kb_row_adc[i];
        } else if (kb_row_adc[i] > (kb_sw_adc[i][kb_col_num] + press_step)) {
            kb_row_read |= 0x1 << i;
            kb_sw_adc[i][kb_col_num] = kb_row_adc[i];
        } else if (kb_row_adc[i] < (kb_sw_adc[i][kb_col_num] - release_step)) {
            kb_row_read &= ~(0x1 << i);
            kb_sw_adc[i][kb_col_num] = kb_row_adc[i];
        } else {
            if (kb_key_state[i][kb_col_num] != 0) {
                kb_row_read |= 0x1 << i;
            } else {
                kb_row_read &= ~(0x1 << i);
            }
        }
    }
//    printf("r[][%d]:%d\n", kb_col_num, kb_row_read);
}

void kb_row_adc_trigger() {
    const static uint16_t max = 4096 * 0.7;
    const static uint16_t min = 4096 * 0.3;

    kb_row_read = 0;
    for (int i = 0; i < KB_ROW_NUM; ++i) {
        if (kb_row_adc[i] > max) {
            kb_row_read |= 0x1 << i;
        } else if (kb_row_adc[i] < min) {
            kb_row_read &= ~(0x1 << i);
        }
    }
}