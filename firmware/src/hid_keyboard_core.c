/*!
    \file    standard_hid_core.c
    \brief   HID class driver

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

#include "hid_keyboard_core.h"
#include <string.h>

#define USBD_VID                     0x28e9U
//#define USBD_PID                     0x028AU
#if KB_SIDE == LEFT
    #define USBD_PID 0x038aU
#else
    #define USBD_PID 0x038bU
#endif

/* Note:it should use the C99 standard when compiling the below codes */
/* USB device descriptor */
const usb_desc_dev hid_kb_dev_desc =
{
    .header =
     {
         .bLength          = USB_DEV_DESC_LEN,
         .bDescriptorType  = USB_DESCTYPE_DEV
     },
    .bcdUSB                = HID_USB_RELEASE,
    .bDeviceClass          = 0x00U,
    .bDeviceSubClass       = 0x00U,
    .bDeviceProtocol       = 0x00U,
    .bMaxPacketSize0       = USB_FS_EP0_MAX_LEN,
    .idVendor              = USBD_VID,
    .idProduct             = USBD_PID,
//    .bcdDevice             = 0x0100U,
    .bcdDevice             = HID_DEVICE,
    .iManufacturer         = STR_IDX_MFC,
    .iProduct              = STR_IDX_PRODUCT,
    .iSerialNumber         = STR_IDX_SERIAL,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};


const usb_hid_desc_config_set hid_kb_config_desc =
{
    // usb configuration descriptor
    .config =
    {
        .header =
         {
             .bLength         = USB_CFG_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_CONFIG
         },
        .wTotalLength         = HID_KB_CONFIG_DESC_LEN,
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x00U,
//        .bmAttributes         = 0xA0U,
//        .bMaxPower            = 0x80U
//        .bMaxPower            = 0xffU
        .bmAttributes         = HID_BUS_POWERED | HID_REMOTEWAKEUP,
        .bMaxPower            = HID_MAXPOWER(500)
    },

    // interface descriptor
    .hid_itf =
    {
        .header =
        {
            .bLength         = USB_ITF_DESC_LEN,
            .bDescriptorType = USB_DESCTYPE_ITF
        },
        .bInterfaceNumber     = HID_KB_INF,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x02U,
        .bInterfaceClass      = USB_HID_CLASS,
        .bInterfaceSubClass   = USB_HID_SUBCLASS_BOOT_ITF,
        .bInterfaceProtocol   = USB_HID_PROTOCOL_KEYBOARD,
        .iInterface           = 0x00U
    },

    // hid descriptor
    .hid_vendor =
    {
        .header =
        {
            .bLength         = USB_HID_DESC_LEN,
            .bDescriptorType = USB_DESCTYPE_HID
        },
        .bcdHID               = HID_CD,
        .bCountryCode         = HID_COUNTRYCODE,
        .bNumDescriptors      = 0x01U,
        .bDescriptorType      = USB_DESCTYPE_REPORT,
        .wDescriptorLength    = HID_KB_REPORT_DESC_LEN,
    },

    // endpoint descriptor
    .hid_epin =
    {
        .header =
        {
            .bLength         = USB_EP_DESC_LEN,
            .bDescriptorType = USB_DESCTYPE_EP
        },
        .bEndpointAddress     = HID_KB_IN_EP,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = HID_KB_IN_PACKET,
        .bInterval            = 0x01U
    },

    .hid_epout =
    {
        .header =
        {
            .bLength         = USB_EP_DESC_LEN,
            .bDescriptorType = USB_DESCTYPE_EP
        },
        .bEndpointAddress     = HID_KB_OUT_EP,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = HID_KB_OUT_PACKET,
        .bInterval            = 0x01U
    }
};

/* USB language ID Descriptor */
const usb_desc_LANGID usbd_kb_language_id_desc =
{
    .header =
     {
         .bLength         = sizeof(usb_desc_LANGID),
         .bDescriptorType = USB_DESCTYPE_STR
     },
    .wLANGID              = ENG_LANGID
};

/* USB manufacture string */
static const usb_desc_str manufacturer_string =
{
    .header =
     {
         .bLength         = USB_STRING_LEN(10U),
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'i', 'g', 'a', 'D', 'e', 'v', 'i', 'c', 'e'}
};

/* USB product string */
static const usb_desc_str product_string =
{
    .header =
     {
         .bLength         = USB_STRING_LEN(5U),
         .bDescriptorType = USB_DESCTYPE_STR,
     },
//    .unicode_string = {'G', 'D', '3', '2', '-','U', 'S', 'B', '_', 'K', 'e', 'y', 'b', 'o', 'a', 'r', 'd'}
     .unicode_string = {'S', 'O', 'K', '4', '2'}
};

/* USBD serial string */
static usb_desc_str serial_string =
{
    .header =
     {
         .bLength         = USB_STRING_LEN(12U),
         .bDescriptorType = USB_DESCTYPE_STR,
     }
};

void *const usbd_hid_kb_strings[] =
{
    [STR_IDX_LANGID]  = (uint8_t *)&usbd_kb_language_id_desc,
    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string
};

usb_desc hid_kb_desc = {
    .dev_desc    = (uint8_t *)&hid_kb_dev_desc,
    .config_desc = (uint8_t *)&hid_kb_config_desc,
    .strings     = usbd_hid_kb_strings
};

/* report desc */
// bitmap
const uint8_t hid_kb_report_desc[HID_KB_REPORT_DESC_LEN] = {
    USAGE_PAGE, USAGE_PAGE_GENERIC_DESKTOP,
    USAGE, USAGE_GENERIC_DESKTOP_PAGE_KEYBOARD,
    COLLECTION, COLLECTION_APPLICATION,
        // modifier keys
        USAGE_PAGE, USAGE_PAGE_KEYBOARD,
        USAGE_MINIMUM, KB_PAGE_LEFTCONTROL,
        USAGE_MAXIMUM, KB_PAGE_RIGHT_GUI,
        LOGICAL_MINIMUM, 0x00,
        LOGICAL_MAXIMUM, 0x01,
        REPORT_SIZE, 0x01,
        REPORT_COUNT, 0x08,
        USAGE_TYPE_INPUT, USAGE_TYPE_DATA_DV,
        // normal keys
        USAGE_PAGE, USAGE_PAGE_KEYBOARD,
        USAGE_MINIMUM, KB_PAGE_NONE,
        USAGE_MAXIMUM, KB_PAGE_KEYPAD_PLUS,  // REPORT_COUNT - 1
        LOGICAL_MINIMUM, 0x00,
        LOGICAL_MAXIMUM, 0x01,
        REPORT_SIZE, 0x01,
        REPORT_COUNT, 0x58,
        USAGE_TYPE_INPUT, USAGE_TYPE_DATA_DV,
        // leds
        USAGE_PAGE, USAGE_PAGE_LED,
        USAGE_MINIMUM, LED_PAGE_UNDEFINED,
        USAGE_MAXIMUM, LED_PAGE_SHIFT,
        REPORT_SIZE, 0x01,
        REPORT_COUNT, 0x08,
        USAGE_TYPE_OUTPUT, USAGE_TYPE_DATA_DV,
    END_COLECTION

};


/* local function prototypes ('static') */
static uint8_t hid_kb_init    (usb_dev *udev, uint8_t config_index);
static uint8_t hid_kb_deinit  (usb_dev *udev, uint8_t config_index);
static uint8_t hid_kb_req     (usb_dev *udev, usb_req *req);
static uint8_t hid_kb_data_in (usb_dev *udev, uint8_t ep_num);
static uint8_t hid_kb_data_out (usb_dev *udev, uint8_t ep_num);

usb_class_core hid_kb_cb = {
    .command         = NO_CMD,
    .alter_set       = 0U,

    .init            = hid_kb_init,
    .deinit          = hid_kb_deinit,
    .req_proc        = hid_kb_req,
    .data_in         = hid_kb_data_in,
    .data_out        = hid_kb_data_out
};

/*!
    \brief      check cdc acm is ready for data transfer
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     0 if cdc is ready, 5 else
*/
uint8_t hid_kb_check_send(usb_dev *udev)
{
    hid_kb_handler *hid = (hid_kb_handler *)udev->dev.class_data[HID_KB_INF];

    if (1U == hid->sent_flag) {
        return USBD_OK;
    }

    return USBD_BUSY;
}

uint8_t hid_kb_check_recev(usb_dev *udev)
{
    hid_kb_handler *hid = (hid_kb_handler *)udev->dev.class_data[HID_KB_INF];

    if (1U == hid->receive_flag) {
        return USBD_OK;
    }

    return USBD_BUSY;
}

/*!
    \brief      register HID interface operation functions
    \param[in]  udev: pointer to USB device instance
    \param[in]  hid_fop: HID operation functions structure
    \param[out] none
    \retval     USB device operation status
*/
uint8_t hid_kb_itfop_register (usb_dev *udev, hid_kb_fop_handler *hid_fop)
{
    if (NULL != hid_fop) {
        udev->dev.user_data = (void *)hid_fop;

        return USBD_OK;
    }

    return USBD_FAIL;
}

/*!
    \brief      send keyboard report
    \param[in]  udev: pointer to USB device instance
    \param[in]  report: pointer to HID report
    \param[in]  len: data length
    \param[out] none
    \retval     USB device operation status
*/
uint8_t hid_kb_report_send (usb_dev *udev, uint8_t *report/*, uint32_t len*/)
{
    hid_kb_handler *hid = (hid_kb_handler *)udev->dev.class_data[HID_KB_INF];

    hid->sent_flag = 0U;

    usbd_ep_send(udev, HID_KB_IN_EP, report, HID_KB_SEND_SIZE);

    return USBD_OK;
}

/*!
    \brief      receive CDC ACM data
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     USB device operation status
*/
uint8_t hid_kb_report_receive (usb_dev *udev)
{
    hid_kb_handler *hid = (hid_kb_handler *)udev->dev.class_data[HID_KB_INF];
    hid->receive_flag = 0U;
    usbd_ep_recev(udev, CDC_DATA_OUT_EP, kb_buf_recev, HID_KB_RECEV_SIZE);
    return USBD_OK;
}

/*!
    \brief      initialize the HID device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_kb_init (usb_dev *udev, uint8_t config_index)
{
    static hid_kb_handler hid_handler;

    memset((void *)&hid_handler, 0U, sizeof(hid_kb_handler));

    /* Initialize the data Tx endpoint */
    usbd_ep_setup (udev, &(hid_kb_config_desc.hid_epin));
    usbd_ep_setup (udev, &(hid_kb_config_desc.hid_epout));

    usbd_ep_recev (udev, HID_KB_OUT_EP, hid_handler.data_out, HID_KB_RECEV_SIZE);

    hid_handler.sent_flag = 1U;
    hid_handler.receive_flag = 1U;

    udev->dev.class_data[HID_KB_INF] = (void *)&hid_handler;

    if (NULL != udev->dev.user_data) {
        ((hid_kb_fop_handler *)udev->dev.user_data)->hid_itf_config();
    }

    return USBD_OK;
}

/*!
    \brief      de-initialize the HID device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_kb_deinit (usb_dev *udev, uint8_t config_index)
{
    /* deinitialize HID endpoints */
    usbd_ep_clear(udev, HID_KB_IN_EP);
    usbd_ep_clear(udev, HID_KB_OUT_EP);

    return USBD_OK;
}

/*!
    \brief      handle the HID class-specific requests
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: device class-specific request
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_kb_req (usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->dev.transc_in[0];

    hid_kb_handler *hid = (hid_kb_handler *)udev->dev.class_data[HID_KB_INF];

    switch (req->bRequest) {
        case GET_REPORT:
            /* no use for this driver */
            break;

        case GET_IDLE:
            transc->xfer_buf = (uint8_t *)&hid->idle_state;

            transc->remain_len = 1U;
            break;

        case GET_PROTOCOL:
            transc->xfer_buf = (uint8_t *)&hid->protocol;

            transc->remain_len = 1U;
            break;

        case SET_REPORT:
            /* no use for this driver */
            break;

        case SET_IDLE:
            hid->idle_state = (uint8_t)(req->wValue >> 8U);
            break;

        case SET_PROTOCOL:
            hid->protocol = (uint8_t)(req->wValue);
            break;

        case USB_GET_DESCRIPTOR:
            if (USB_DESCTYPE_REPORT == (req->wValue >> 8U)) {
                transc->remain_len = USB_MIN(HID_KB_REPORT_DESC_LEN, req->wLength);
                transc->xfer_buf = (uint8_t *)hid_kb_report_desc;

                return REQ_SUPP;
            }
            break;

        default:
            break;
    }

    return USBD_OK;
}

/*!
    \brief      handle data stage
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint identifier
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_kb_data_in (usb_dev *udev, uint8_t ep_num)
{

    hid_kb_handler *hid = (hid_kb_handler *)udev->dev.class_data[HID_KB_INF];

    hid->sent_flag = 1U;

    return USBD_OK;
}

/*!
    \brief      handle custom HID data
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint identifier
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_kb_data_out (usb_dev *udev, uint8_t ep_num)
{
    hid_kb_handler *hid = (hid_kb_handler *)udev->dev.class_data[HID_KB_INF];

    hid->receive_flag = 1U;

    return USBD_OK;
}
