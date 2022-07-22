#ifndef LED_H
#define LED_H

//#include "main.h"
#include "keyboard.h"

#if !defined(KB_ROW_NUM) && !defined(KB_COL_NUM)
    #define KB_ROW_NUM 6U
    #define KB_COL_NUM 7U
#endif

//#define led_k_num 42U
#define led_s_num 4U
#define led_e_num 18U
//#define led_color_num 3U

//enum led {
//    key, edge, sys
//};

enum led_color {
    b, g, r
};

enum led_reg {
    pwm, scale, func
};

//#define cmd_reg_unlock_size 2U
//#define reg_sel_size 2U
//#define led_reg_page0_size 0xb5U
//#define led_reg_page1_size 0xacU

#define led_pwm_size 0xc6U
#define led_func_size 0x04U
// automatic address increment: 0x01 cmd + 0x01 reg first addr + 0xc6 data
//#define led_data_size 0xc8U
#define led_data_size (led_pwm_size + 2)
//#define led_all_size 0x194U

#define led_pwm_page 0x50U
#define led_scale_page 0x51U
#define led_func_page 0x52U
#define led_first_addr_data 0x01U
#define led_first_addr_func 0x00U
#define led_gcc 0x10U


//typedef struct {
//    uint8_t cmd;
//    uint8_t addr;
//    uint8_t data[];
//}led_std_struct;

typedef struct {
    uint8_t cmd;
    uint8_t addr;
    uint8_t data[led_data_size];
}led_pwm_struct, led_scale_struct;

typedef struct {
    uint8_t cmd;
    uint8_t addr;
    uint8_t data[led_func_size];
}led_func_struct;

/*typedef struct {
    led_struct *_led_pwm;
    led_struct *_led_scale;
    led_struct *_led_func;
}led_all_struct;*/

extern led_pwm_struct led_pwm_reg, led_scale_reg;
extern led_func_struct led_func_reg;
//extern led_all_struct led_all;

extern uint8_t led_effect;

extern uint32_t LED_FLAG;
#define LED_FLAG_CHANGE BIT(0)

enum led_ctl {
    LED_OFF = (uint8_t)0x00,
    LED_ON,
    LED_NO,
    LED_SPECTRUM,
    LED_STATIC,
    LED_REACTIVE,
    LED_BREATH
};

void led_polling();

void leds_init();
void leds_on();
void leds_off();
//void leds_func_trans();
//void leds_pwm_trans();
//void leds_scale_trans();
//void leds_all_trans();
//void leds_color(uint8_t mode, uint8_t color, uint8_t color_val);
void leds_colors(uint8_t mode, uint8_t r_val, uint8_t g_val, uint8_t b_val);
void led_colors_set(uint8_t mode, uint8_t r_val, uint8_t g_val, uint8_t b_val, const uint8_t *led_which);
void leds_test();

void led_dma(uint8_t mode);
//void led_dma_start(uint8_t mode);
//void led_dma_finish();
void leds_spectrum();
void leds_reactive();

enum is31fl3743b {
    CS1_SW1 = (uint8_t)0x00,
    CS2_SW1,
    CS3_SW1,
    CS4_SW1,
    CS5_SW1,
    CS6_SW1,
    CS7_SW1,
    CS8_SW1,
    CS9_SW1,
    CS10_SW1,
    CS11_SW1,
    CS12_SW1,
    CS13_SW1,
    CS14_SW1,
    CS15_SW1,
    CS16_SW1,
    CS17_SW1,
    CS18_SW1,

    CS1_SW2,
    CS2_SW2,
    CS3_SW2,
    CS4_SW2,
    CS5_SW2,
    CS6_SW2,
    CS7_SW2,
    CS8_SW2,
    CS9_SW2,
    CS10_SW2,
    CS11_SW2,
    CS12_SW2,
    CS13_SW2,
    CS14_SW2,
    CS15_SW2,
    CS16_SW2,
    CS17_SW2,
    CS18_SW2,

    CS1_SW3,
    CS2_SW3,
    CS3_SW3,
    CS4_SW3,
    CS5_SW3,
    CS6_SW3,
    CS7_SW3,
    CS8_SW3,
    CS9_SW3,
    CS10_SW3,
    CS11_SW3,
    CS12_SW3,
    CS13_SW3,
    CS14_SW3,
    CS15_SW3,
    CS16_SW3,
    CS17_SW3,
    CS18_SW3,

    CS1_SW4,
    CS2_SW4,
    CS3_SW4,
    CS4_SW4,
    CS5_SW4,
    CS6_SW4,
    CS7_SW4,
    CS8_SW4,
    CS9_SW4,
    CS10_SW4,
    CS11_SW4,
    CS12_SW4,
    CS13_SW4,
    CS14_SW4,
    CS15_SW4,
    CS16_SW4,
    CS17_SW4,
    CS18_SW4,

    CS1_SW5,
    CS2_SW5,
    CS3_SW5,
    CS4_SW5,
    CS5_SW5,
    CS6_SW5,
    CS7_SW5,
    CS8_SW5,
    CS9_SW5,
    CS10_SW5,
    CS11_SW5,
    CS12_SW5,
    CS13_SW5,
    CS14_SW5,
    CS15_SW5,
    CS16_SW5,
    CS17_SW5,
    CS18_SW5,

    CS1_SW6,
    CS2_SW6,
    CS3_SW6,
    CS4_SW6,
    CS5_SW6,
    CS6_SW6,
    CS7_SW6,
    CS8_SW6,
    CS9_SW6,
    CS10_SW6,
    CS11_SW6,
    CS12_SW6,
    CS13_SW6,
    CS14_SW6,
    CS15_SW6,
    CS16_SW6,
    CS17_SW6,
    CS18_SW6,

    CS1_SW7,
    CS2_SW7,
    CS3_SW7,
    CS4_SW7,
    CS5_SW7,
    CS6_SW7,
    CS7_SW7,
    CS8_SW7,
    CS9_SW7,
    CS10_SW7,
    CS11_SW7,
    CS12_SW7,
    CS13_SW7,
    CS14_SW7,
    CS15_SW7,
    CS16_SW7,
    CS17_SW7,
    CS18_SW7,

    CS1_SW8,
    CS2_SW8,
    CS3_SW8,
    CS4_SW8,
    CS5_SW8,
    CS6_SW8,
    CS7_SW8,
    CS8_SW8,
    CS9_SW8,
    CS10_SW8,
    CS11_SW8,
    CS12_SW8,
    CS13_SW8,
    CS14_SW8,
    CS15_SW8,
    CS16_SW8,
    CS17_SW8,
    CS18_SW8,

    CS1_SW9,
    CS2_SW9,
    CS3_SW9,
    CS4_SW9,
    CS5_SW9,
    CS6_SW9,
    CS7_SW9,
    CS8_SW9,
    CS9_SW9,
    CS10_SW9,
    CS11_SW9,
    CS12_SW9,
    CS13_SW9,
    CS14_SW9,
    CS15_SW9,
    CS16_SW9,
    CS17_SW9,
    CS18_SW9,

    CS1_SW10,
    CS2_SW10,
    CS3_SW10,
    CS4_SW10,
    CS5_SW10,
    CS6_SW10,
    CS7_SW10,
    CS8_SW10,
    CS9_SW10,
    CS10_SW10,
    CS11_SW10,
    CS12_SW10,
    CS13_SW10,
    CS14_SW10,
    CS15_SW10,
    CS16_SW10,
    CS17_SW10,
    CS18_SW10,

    CS1_SW11,
    CS2_SW11,
    CS3_SW11,
    CS4_SW11,
    CS5_SW11,
    CS6_SW11,
    CS7_SW11,
    CS8_SW11,
    CS9_SW11,
    CS10_SW11,
    CS11_SW11,
    CS12_SW11,
    CS13_SW11,
    CS14_SW11,
    CS15_SW11,
    CS16_SW11,
    CS17_SW11,
    CS18_SW11,
};

#define LED_EDG10_B CS1_SW1
#define LED_EDG10_G CS2_SW1
#define LED_EDG10_R CS3_SW1
#define LED_EDG12_B CS4_SW1
#define LED_EDG12_G CS5_SW1
#define LED_EDG12_R CS6_SW1
#define LED_SYS0_B CS7_SW1
#define LED_SYS0_G CS8_SW1
#define LED_SYS0_R CS9_SW1
#define LED_EDG0_B CS10_SW1
#define LED_EDG0_G CS11_SW1
#define LED_EDG0_R CS12_SW1
#define LED_EDG6_B CS13_SW1
#define LED_EDG6_G CS14_SW1
#define LED_EDG6_R CS15_SW1
#define LED_EDG8_B CS16_SW1
#define LED_EDG8_G CS17_SW1
#define LED_EDG8_R CS18_SW1

#define LED_EDG11_B CS1_SW2
#define LED_EDG11_G CS2_SW2
#define LED_EDG11_R CS3_SW2
#define LED_EDG17_B CS4_SW2
#define LED_EDG17_G CS5_SW2
#define LED_EDG17_R CS6_SW2
#define LED_SYS1_B CS7_SW2
#define LED_SYS1_G CS8_SW2
#define LED_SYS1_R CS9_SW2
#define LED_EDG5_B CS10_SW2
#define LED_EDG5_G CS11_SW2
#define LED_EDG5_R CS12_SW2
#define LED_EDG7_B CS13_SW2
#define LED_EDG7_G CS14_SW2
#define LED_EDG7_R CS15_SW2
#define LED_EDG9_B CS16_SW2
#define LED_EDG9_G CS17_SW2
#define LED_EDG9_R CS18_SW2

#define LED_EDG14_B CS1_SW3
#define LED_EDG14_G CS2_SW3
#define LED_EDG14_R CS3_SW3
#define LED_EDG13_B CS4_SW3
#define LED_EDG13_G CS5_SW3
#define LED_EDG13_R CS6_SW3
#define LED_SYS2_B CS7_SW3
#define LED_SYS2_G CS8_SW3
#define LED_SYS2_R CS9_SW3
#define LED_EDG1_B CS10_SW3
#define LED_EDG1_G CS11_SW3
#define LED_EDG1_R CS12_SW3
#define LED_EDG2_B CS13_SW3
#define LED_EDG2_G CS14_SW3
#define LED_EDG2_R CS15_SW3
//#define LED_KEY1_B CS16_SW3
//#define LED_KEY1_G CS17_SW3
//#define LED_KEY1_R CS18_SW3

#define LED_EDG15_B CS1_SW4
#define LED_EDG15_G CS2_SW4
#define LED_EDG15_R CS3_SW4
#define LED_EDG16_B CS4_SW4
#define LED_EDG16_G CS5_SW4
#define LED_EDG16_R CS6_SW4
#define LED_SYS3_B CS7_SW4
#define LED_SYS3_G CS8_SW4
#define LED_SYS3_R CS9_SW4
#define LED_EDG4_B CS10_SW4
#define LED_EDG4_G CS11_SW4
#define LED_EDG4_R CS12_SW4
#define LED_EDG3_B CS13_SW4
#define LED_EDG3_G CS14_SW4
#define LED_EDG3_R CS15_SW4
//#define LED_KEY1_B CS16_SW4
//#define LED_KEY1_G CS17_SW4
//#define LED_KEY1_R CS18_SW4

#define LED_KEY21_B CS1_SW5
#define LED_KEY21_G CS2_SW5
#define LED_KEY21_R CS3_SW5
#define LED_KEY28_B CS4_SW5
#define LED_KEY28_G CS5_SW5
#define LED_KEY28_R CS6_SW5
#define LED_KEY35_B CS7_SW5
#define LED_KEY35_G CS8_SW5
#define LED_KEY35_R CS9_SW5
#define LED_KEY0_B CS10_SW5
#define LED_KEY0_G CS11_SW5
#define LED_KEY0_R CS12_SW5
#define LED_KEY7_B CS13_SW5
#define LED_KEY7_G CS14_SW5
#define LED_KEY7_R CS15_SW5
#define LED_KEY14_B CS16_SW5
#define LED_KEY14_G CS17_SW5
#define LED_KEY14_R CS18_SW5

#define LED_KEY22_B CS1_SW6
#define LED_KEY22_G CS2_SW6
#define LED_KEY22_R CS3_SW6
#define LED_KEY29_B CS4_SW6
#define LED_KEY29_G CS5_SW6
#define LED_KEY29_R CS6_SW6
#define LED_KEY36_B CS7_SW6
#define LED_KEY36_G CS8_SW6
#define LED_KEY36_R CS9_SW6
#define LED_KEY1_B CS10_SW6
#define LED_KEY1_G CS11_SW6
#define LED_KEY1_R CS12_SW6
#define LED_KEY8_B CS13_SW6
#define LED_KEY8_G CS14_SW6
#define LED_KEY8_R CS15_SW6
#define LED_KEY15_B CS16_SW6
#define LED_KEY15_G CS17_SW6
#define LED_KEY15_R CS18_SW6

#define LED_KEY23_B CS1_SW7
#define LED_KEY23_G CS2_SW7
#define LED_KEY23_R CS3_SW7
#define LED_KEY30_B CS4_SW7
#define LED_KEY30_G CS5_SW7
#define LED_KEY30_R CS6_SW7
#define LED_KEY37_B CS7_SW7
#define LED_KEY37_G CS8_SW7
#define LED_KEY37_R CS9_SW7
#define LED_KEY2_B CS10_SW7
#define LED_KEY2_G CS11_SW7
#define LED_KEY2_R CS12_SW7
#define LED_KEY9_B CS13_SW7
#define LED_KEY9_G CS14_SW7
#define LED_KEY9_R CS15_SW7
#define LED_KEY16_B CS16_SW7
#define LED_KEY16_G CS17_SW7
#define LED_KEY16_R CS18_SW7

#define LED_KEY24_B CS1_SW8
#define LED_KEY24_G CS2_SW8
#define LED_KEY24_R CS3_SW8
#define LED_KEY31_B CS4_SW8
#define LED_KEY31_G CS5_SW8
#define LED_KEY31_R CS6_SW8
#define LED_KEY38_B CS7_SW8
#define LED_KEY38_G CS8_SW8
#define LED_KEY38_R CS9_SW8
#define LED_KEY3_B CS10_SW8
#define LED_KEY3_G CS11_SW8
#define LED_KEY3_R CS12_SW8
#define LED_KEY10_B CS13_SW8
#define LED_KEY10_G CS14_SW8
#define LED_KEY10_R CS15_SW8
#define LED_KEY17_B CS16_SW8
#define LED_KEY17_G CS17_SW8
#define LED_KEY17_R CS18_SW8

#define LED_KEY25_B CS1_SW9
#define LED_KEY25_G CS2_SW9
#define LED_KEY25_R CS3_SW9
#define LED_KEY32_B CS4_SW9
#define LED_KEY32_G CS5_SW9
#define LED_KEY32_R CS6_SW9
#define LED_KEY39_B CS7_SW9
#define LED_KEY39_G CS8_SW9
#define LED_KEY39_R CS9_SW9
#define LED_KEY4_B CS10_SW9
#define LED_KEY4_G CS11_SW9
#define LED_KEY4_R CS12_SW9
#define LED_KEY11_B CS13_SW9
#define LED_KEY11_G CS14_SW9
#define LED_KEY11_R CS15_SW9
#define LED_KEY18_B CS16_SW9
#define LED_KEY18_G CS17_SW9
#define LED_KEY18_R CS18_SW9

#define LED_KEY26_B CS1_SW10
#define LED_KEY26_G CS2_SW10
#define LED_KEY26_R CS3_SW10
#define LED_KEY33_B CS4_SW10
#define LED_KEY33_G CS5_SW10
#define LED_KEY33_R CS6_SW10
#define LED_KEY40_B CS7_SW10
#define LED_KEY40_G CS8_SW10
#define LED_KEY40_R CS9_SW10
#define LED_KEY5_B CS10_SW10
#define LED_KEY5_G CS11_SW10
#define LED_KEY5_R CS12_SW10
#define LED_KEY12_B CS13_SW10
#define LED_KEY12_G CS14_SW10
#define LED_KEY12_R CS15_SW10
#define LED_KEY19_B CS16_SW10
#define LED_KEY19_G CS17_SW10
#define LED_KEY19_R CS18_SW10

#define LED_KEY27_B CS1_SW11
#define LED_KEY27_G CS2_SW11
#define LED_KEY27_R CS3_SW11
#define LED_KEY34_B CS4_SW11
#define LED_KEY34_G CS5_SW11
#define LED_KEY34_R CS6_SW11
#define LED_KEY41_B CS7_SW11
#define LED_KEY41_G CS8_SW11
#define LED_KEY41_R CS9_SW11
#define LED_KEY6_B CS10_SW11
#define LED_KEY6_G CS11_SW11
#define LED_KEY6_R CS12_SW11
#define LED_KEY13_B CS13_SW11
#define LED_KEY13_G CS14_SW11
#define LED_KEY13_R CS15_SW11
#define LED_KEY20_B CS16_SW11
#define LED_KEY20_G CS17_SW11
#define LED_KEY20_R CS18_SW11


static const uint8_t led_key[KB_ROW_NUM][KB_COL_NUM][3];
static const uint8_t led_edg[led_e_num][3];
static const uint8_t led_sys[led_s_num][3];


#endif //LED_H
