/*!
    \file    usbd_conf.h
    \brief   the header file of USB device-mode configuration

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

#ifndef USBD_CONF_H
#define USBD_CONF_H

#include "usb_conf.h"
#include "drv_usb_regs.h"

#define USBD_CFG_MAX_NUM                    1U
#define USBD_ITF_MAX_NUM                    3U
//#define USBD_ITF_MAX_NUM                    1U

//#define USB_STR_DESC_MAX_SIZE               64

//#define USB_STRING_COUNT                    4U

#define HID_KB_INF 0x00U
#define HID_MOUSE_INF 0x01U
#define HID_CNTLR_INF 0x02U

#define CDC_COM_INTERFACE 0x00U
#define CDC_DATA_INTERFACE 0x02U

#define USBD_DFU_INTERFACE           0

/* endpoint count used by the HID device */

#define HID_KB_IN_EP                           EP1_IN
#define HID_KB_OUT_EP                          EP1_OUT

#define HID_MOUSE_IN_EP                           EP2_IN
#define HID_MOUSE_OUT_EP                          EP2_OUT

#define HID_CNTLR_IN_EP                           EP3_IN
#define HID_CNTLR_OUT_EP                          EP3_OUT

#define CDC_CMD_EP                          EP2_IN
#define CDC_DATA_IN_EP                           EP1_IN
#define CDC_DATA_OUT_EP                          EP3_OUT

/* DFU endpoint define */
#define DFU_IN_EP                    EP0_IN
#define DFU_OUT_EP                   EP0_OUT

//#define HID_IN_PACKET                       64
//#define HID_IN_PACKET                       8
//#define HID_OUT_PACKET                      8

#define HID_KB_IN_PACKET                       64
#define HID_KB_OUT_PACKET                      64

#define HID_MOUSE_IN_PACKET                       8
#define HID_MOUSE_OUT_PACKET                      8

#define HID_CNTLR_IN_PACKET                       8
#define HID_CNTLR_OUT_PACKET                      8

//#define HID_COMPOSITE_IN_PACKET                       8
//#define HID_COMPOSITE_OUT_PACKET                      8

#define USB_CDC_CMD_PACKET_SIZE             8
#define USB_CDC_DATA_PACKET_SIZE            64

/* Maximum number of supported media (Flash) */
#define MAX_USED_MEMORY_MEDIA        1

//#define USB_STRING_COUNT             6U

/* DFU maximum data packet size */
#define TRANSFER_SIZE                2048

/* memory address from where user application will be loaded, which represents
   the dfu code protected against write and erase operations.*/
#define APP_LOADED_ADDR              0x08010000

/* Make sure the corresponding memory where the DFU code should not be loaded
   cannot be erased or overwritten by DFU application. */
#define IS_PROTECTED_AREA(addr)      (uint8_t)(((addr >= 0x08000000) && (addr < (APP_LOADED_ADDR)))? 1 : 0)

#endif /* USBD_CONF_H */
