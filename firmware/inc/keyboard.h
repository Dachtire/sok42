#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "usb_conf.h"
#include "main.h"
#include "gd32vf103_it.h"
//#include "gd32vf103.h"

#define LEFT 0
#define RIGHT 1
#define KB_SIDE LEFT
//#define KB_SIDE RIGHT
#define SOLO 0

#define HID_KB_SEND_SIZE 12U  // bytes
#define HID_KB_RECEV_SIZE 12U  // bytes
#define HID_CNTLR_BUF_SIZE 6U  // bytes
#define HID_MICE_BUF_SIZE 4U  // bytes


#define KB_LYR_NUM 4U
#define KB_ROW_NUM 6U
#define KB_COL_NUM 7U

#define KB_CTL_FN BIT(0)
#define KB_CTL_KEYBOARD BIT(1)
#define KB_CTL_MOUSE BIT(2)
#define KB_CTL_CNTLR BIT(3)
#define KB_CTL_USB BIT(16)
#define KB_CTL_USART BIT(17)
#define KB_CTL_USB_KB BIT(18)
#define KB_CTL_MASTER BIT(19)
#define KB_CTL_REPEAT BIT(20)
#define KB_CTL_LED BIT(21)
#define KB_CTL_USB_COMPOSITE BIT(22)
#define KB_CTL_USB_CDC BIT(23)
//#define KB_CTL_ADC BIT(24)

enum kb_fn_flag {
    kb_fn_none = (uint8_t)0,
    kb_fn_lyr,
    kb_fn_led,
    kb_fn_uart,
    kb_fn_ms,
    kb_fn_usb,
};

#define KB_FLAG_REPEAT BIT(0)
#define KB_FLAG_COUNT BIT(1)
//#define KB_FLAG_DEBUG BIT(2)

#define CNTLR_STICK_VALUE 127

void kb_remap(uint8_t lyr);
//void kb_usb_buf_handler();
void kb_usb_send();
void kb_fn_handler();
void kb_init_sync();
void kb_init();
void kb_polling(/*uint8_t col*/);
void kb_usart_polling();
void kb_it();
void kb_uart_init();
void adc_it();
void kb_usart_debug();


extern uint8_t kb_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM], kb_cntlr_lyrs[KB_LYR_NUM][KB_ROW_NUM][KB_COL_NUM];

extern uint8_t *kb_usb_buf;
//extern *usb_hid_cntlr_buf;
extern uint8_t kb_buf_mix[HID_KB_SEND_SIZE], kb_buf_master[HID_KB_SEND_SIZE], kb_buf_slave[HID_KB_SEND_SIZE], kb_buf_recev[HID_KB_RECEV_SIZE], kb_buf_empty[HID_KB_SEND_SIZE];
//struct cntlr_buf_type {
//    uint16_t button;
//    int8_t stick[4];
//};
//extern struct cntlr_buf_type cntlr_buf;
extern uint8_t kb_buf_cntrl[];
extern uint8_t kb_buf_mouse[];
extern uint8_t kb_col_num, kb_row_num;
extern uint8_t (*kb_layout)[KB_COL_NUM];
extern bool kb_key_state[KB_ROW_NUM][KB_COL_NUM];
extern uint32_t KB_CTL;
extern uint32_t KB_FLAG;
extern uint8_t kb_key_count;


#define KB_RELESE ((bool)0)
#define KB_PRESS ((bool)1)

enum kb_keycode {
    KEY_LEFT_CONTROL = (uint8_t)0x00,
    KEY_LEFT_SHIFT,
    KEY_LEFT_ALT,
    KEY_LEFT_GUI,
    KEY_RIGHT_CONTROL,
    KEY_RIGHT_SHIFT,
    KEY_RIGHT_ALT,
    KEY_RIGHT_GUI,

    KEY_NONE,  // 0x08
    KEY_ERRORROLLOVER,
    KEY_POSTFAIL,
    KEY_ERRORUNDEFINED,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_1_EXCLAMATION_MARK,
    KEY_2_AT,
    KEY_3_NUMBER_SIGN,
    KEY_4_DOLLAR,
    KEY_5_PERCENT,
    KEY_6_CARET,
    KEY_7_AMPERSAND,
    KEY_8_ASTERISK,
    KEY_9_OPARENTHESIS,
    KEY_0_CPARENTHESIS,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_SPACEBAR,
    KEY_MINUS_UNDERSCORE,
    KEY_EQUAL_PLUS,
    KEY_OBRACKET_AND_OBRACE,
    KEY_CBRACKET_AND_CBRACE,
    KEY_BACKSLASH_VERTICAL_BAR,
    KEY_NONUS_NUMBER_SIGN_TILDE,
    KEY_SEMICOLON_COLON,
    KEY_SINGLE_AND_DOUBLE_QUOTE,
    KEY_GRAVE_ACCENT_AND_TILDE,
    KEY_COMMA_AND_LESS,
    KEY_DOT_GREATER,
    KEY_SLASH_QUESTION,
    KEY_CAPS_LOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_PRINTSCREEN,
    KEY_SCROLL_LOCK,
    KEY_PAUSE,
    KEY_INSERT,
    KEY_HOME,
    KEY_PAGEUP,
    KEY_DELETE,
    KEY_END,
    KEY_PAGEDOWN,
    KEY_RIGHTARROW,
    KEY_LEFTARROW,
    KEY_DOWNARROW,
    KEY_UPARROW,  // 0x5a

    KEY_KEYPAD_NUM_LOCK_AND_CLEAR,
    KEY_KEYPAD_SLASH,
    KEY_KEYPAD_ASTERIKS,
    KEY_KEYPAD_MINUS,
    KEY_KEYPAD_PLUS,
    KEY_KEYPAD_ENTER,

    KEY_KEYPAD_1_END,
    KEY_KEYPAD_2_DOWN_ARROW,
    KEY_KEYPAD_3_PAGEDN,
    KEY_KEYPAD_4_LEFT_ARROW,
    KEY_KEYPAD_5,
    KEY_KEYPAD_6_RIGHT_ARROW,
    KEY_KEYPAD_7_HOME,
    KEY_KEYPAD_8_UP_ARROW,
    KEY_KEYPAD_9_PAGEUP,
    KEY_KEYPAD_0_INSERT,
    KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE,
    KEY_NONUS_BACK_SLASH_VERTICAL_BAR,
    KEY_APPLICATION,
    KEY_POWER,
    KEY_KEYPAD_EQUAL,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,
    KEY_EXECUTE,
    KEY_HELP,
    KEY_MENU,
    KEY_SELECT,
    KEY_STOP,
    KEY_AGAIN,
    KEY_UNDO,
    KEY_CUT,
    KEY_COPY,
    KEY_PASTE,
    KEY_FIND,
    KEY_MUTE,
    KEY_VOLUME_UP,
    KEY_VOLUME_DOWN,
    KEY_LOCKING_CAPS_LOCK,
    KEY_LOCKING_NUM_LOCK,
    KEY_LOCKING_SCROLL_LOCK,
    KEY_KEYPAD_COMMA,
    KEY_KEYPAD_EQUAL_SIGN,
    KEY_INTERNATIONAL1,
    KEY_INTERNATIONAL2,
    KEY_INTERNATIONAL3,
    KEY_INTERNATIONAL4,
    KEY_INTERNATIONAL5,
    KEY_INTERNATIONAL6,
    KEY_INTERNATIONAL7,
    KEY_INTERNATIONAL8,
    KEY_INTERNATIONAL9,
    KEY_LANG1,
    KEY_LANG2,
    KEY_LANG3,
    KEY_LANG4,
    KEY_LANG5,
    KEY_LANG6,
    KEY_LANG7,
    KEY_LANG8,
    KEY_LANG9,
    KEY_ALTERNATE_ERASE,
    KEY_SYSREQ,
    KEY_CANCEL,
    KEY_CLEAR,
    KEY_PRIOR,
    KEY_RETURN,
    KEY_SEPARATOR,
    KEY_OUT,
    KEY_OPER,
    KEY_CLEAR_AGAIN,
    KEY_CRSEL,
    KEY_EXSEL,
    KEY_KEYPAD_00,
    KEY_KEYPAD_000,
    KEY_THOUSANDS_SEPARATOR,
    KEY_DECIMAL_SEPARATOR,
    KEY_CURRENCY_UNIT,
    KEY_CURRENCY_SUB_UNIT,
    KEY_KEYPAD_OPARENTHESIS,
    KEY_KEYPAD_CPARENTHESIS,
    KEY_KEYPAD_OBRACE,
    KEY_KEYPAD_CBRACE,
    KEY_KEYPAD_TAB,
    KEY_KEYPAD_BACKSPACE,
    KEY_KEYPAD_A,
    KEY_KEYPAD_B,
    KEY_KEYPAD_C,
    KEY_KEYPAD_D,
    KEY_KEYPAD_E,
    KEY_KEYPAD_F,
    KEY_KEYPAD_XOR,
    KEY_KEYPAD_CARET,
    KEY_KEYPAD_PERCENT,
    KEY_KEYPAD_LESS,
    KEY_KEYPAD_GREATER,
    KEY_KEYPAD_AMPERSAND,
    KEY_KEYPAD_LOGICAL_AND,
    KEY_KEYPAD_VERTICAL_BAR,
    KEY_KEYPAD_LOGIACL_OR,
    KEY_KEYPAD_COLON,
    KEY_KEYPAD_NUMBER_SIGN,
    KEY_KEYPAD_SPACE,
    KEY_KEYPAD_AT,
    KEY_KEYPAD_EXCLAMATION_MARK,
    KEY_KEYPAD_MEMORY_STORE,
    KEY_KEYPAD_MEMORY_RECALL,
    KEY_KEYPAD_MEMORY_CLEAR,
    KEY_KEYPAD_MEMORY_ADD,
    KEY_KEYPAD_MEMORY_SUBTRACT,
    KEY_KEYPAD_MEMORY_MULTIPLY,
    KEY_KEYPAD_MEMORY_DIVIDE,
    KEY_KEYPAD_PLUSMINUS,
    KEY_KEYPAD_CLEAR,
    KEY_KEYPAD_CLEAR_ENTRY,
    KEY_KEYPAD_BINARY,
    KEY_KEYPAD_OCTAL,
    KEY_KEYPAD_DECIMAL,
    KEY_KEYPAD_HEXADECIMAL, // 0xd9

    KEY_FN = (uint8_t)0xFFU,
};

//#define KEY_FN                          (uint8_t)0xFFU

enum kb_cntlr_code {
/*    CNTLR_BUTTON_1 = (uint8_t) 0x00,
    CNTLR_BUTTON_2,
    CNTLR_BUTTON_3,
    CNTLR_BUTTON_4,
    CNTLR_BUTTON_5,
    CNTLR_BUTTON_6,
    CNTLR_BUTTON_7,
    CNTLR_BUTTON_8,

    CNTLR_BUTTON_9,
    CNTLR_BUTTON_10,
    CNTLR_BUTTON_11,
    CNTLR_BUTTON_12,
    CNTLR_BUTTON_13,
    CNTLR_BUTTON_14,
    CNTLR_BUTTON_15,
    CNTLR_BUTTON_16,

    CNTLR_LEFT_STICK_X_NEG,
    CNTLR_LEFT_STICK_X_POS,
    CNTLR_LEFT_STICK_Y_NEG,
    CNTLR_LEFT_STICK_Y_POS,
    CNTLR_RIGHT_STICK_X_NEG,
    CNTLR_RIGHT_STICK_X_POS,
    CNTLR_RIGHT_STICK_Y_NEG,
    CNTLR_RIGHT_STICK_Y_POS,*/

    CNTLR_A = (uint8_t) 0x00,
    CNTLR_B,
    CNTLR_X,
    CNTLR_Y,
    CNTLR_LEFT_BUMPER,
    CNTLR_RIGHT_BUMPER,
    CNTLR_LEFT_TRIGGER,
    CNTLR_RIGHT_TRIGGER,

    CNTLR_BACK,
    CNTLR_START,
    CNTLR_LEFT_STICK_BUTTON,
    CNTLR_RIGHT_STICK_BUTTON,
    CNTLR_DPAD_UP,
    CNTLR_DPAD_DOWN,
    CNTLR_DPAD_LEFT,
    CNTLR_DPAD_RIGHT,

    CNTLR_LEFT_STICK_LEFT,
    CNTLR_LEFT_STICK_RIGHT,
    CNTLR_LEFT_STICK_UP,
    CNTLR_LEFT_STICK_DOWN,
    CNTLR_RIGHT_STICK_LEFT,
    CNTLR_RIGHT_STICK_RIGHT,
    CNTLR_RIGHT_STICK_UP,
    CNTLR_RIGHT_STICK_DOWN,

    CNTLR_NONE,
};

enum kb_led_hid {
    LED_NONE = (uint8_t)0x00,
    LED_NUM_LOCK,
    LED_CAPS_LOCK,
    LED_SCROLL_LOCK,
    LED_COMPOSE,
    LED_KANA,
    LED_POWER,
    LED_SHIFT,
};


enum kb_ctl_option {
    KB_LYR_0 = (uint8_t)0x80,
    KB_LYR_1,
    KB_LYR_2,
    KB_LYR_3,

    KB_CTL_KB_OFF = (uint8_t)0xc0,
    KB_CTL_KB_ON,

    KB_CTL_REPEAT_OFF = 0xe0,
    KB_CTL_REPEAT_ON
};

#endif /* __KEYBOARD_H */
