#ifndef __USB_KBD_H
#define __USB_KBD_H

#include <stdint.h>
//#include "USB/usbuser.h"

#define USB_IN_EVT              0x0001
#define USB_INIT_EVT            (1<<1)
#define USB_TEST_EVT            0x0008

extern uint8_t USBTaskID;
extern uint8_t USBFSTaskID;

void USB_Task_Init(void);
void USB_Task_deinit(void);

void USBFS_Task_Init(void);
void USBFS_Task_deinit(void);
#endif
