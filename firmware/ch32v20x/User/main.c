/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/08/08
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/ 

/*
 * @Note
 * Composite Keyboard and Mouse Example:
 * This example uses PA4-PA7 and PB12-PB15 to simulate mouse movement and keyboard
 * key pressing respectively, active low. At the same time, it also uses USART2
 * to receive data sent from the host to simulate keyboard key pressing and releasing.
 *
 */

#include "main.h"
/* Global define */

/* Global Variable */

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    //    printf("SystemClk:%d\r\n",SystemCoreClock);
    //    printf("USBD Keyboard&Mouse Demo\r\n");

    kb_init();
    gpio_config();
    kb_init_sync();

    adc_config();
    TIM1_Init();

    Set_USBConfig();
    USB_Init();
    USB_Interrupts_Config();
    //    if( bDeviceState == CONFIGURED ) {
    //        kb_usb = KB_USBD_DEVICE;

    /* Initialize USBFS host */
    #if DEF_USBFS_PORT_EN
        DUG_PRINTF("USBFS Host Init\r\n");
        USBFS_RCC_Init();
        USBFS_Host_Init(ENABLE);
        memset(&RootHubDev.bStatus, 0, sizeof(ROOT_HUB_DEVICE));
        memset(&HostCtl[DEF_USBFS_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL].InterfaceNum, 0, sizeof(HOST_CTL));
    #endif
    //    } else {
    //        /* Initialize USBHD interface to communicate with the host  */
    //        USBFS_RCC_Init( );
    //        USBFS_Device_Init( ENABLE );
    //        USB_Sleep_Wakeup_CFG( );
    //        if (USBFS_DevEnumStatus) {
    //            kb_usb = KB_USBFS_DEVICE;
    //        }
    //    }

    kb_enable_usbd();

    while (1) {
    }
}







