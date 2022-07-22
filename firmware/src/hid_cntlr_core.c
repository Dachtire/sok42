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

#include "hid_cntlr_core.h"
#include <string.h>

#define USBD_VID                     0x28e9U
#define USBD_PID                     0x0389U

/* Note:it should use the C99 standard when compiling the below codes */
/* USB standard device descriptor */
const usb_desc_dev hid_cntlr_dev_desc =
{
    .header =
     {
         .bLength          = USB_DEV_DESC_LEN,
         .bDescriptorType  = USB_DESCTYPE_DEV
     },
    .bcdUSB                = 0x0200U,
    .bDeviceClass          = 0x00U,
    .bDeviceSubClass       = 0x00U,
    .bDeviceProtocol       = 0x00U,
    .bMaxPacketSize0       = USB_FS_EP0_MAX_LEN,
    .idVendor              = USBD_VID,
    .idProduct             = USBD_PID,
    .bcdDevice             = 0x0100U,
    .iManufacturer         = STR_IDX_MFC,
    .iProduct              = STR_IDX_PRODUCT,
    .iSerialNumber         = STR_IDX_SERIAL,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};

const usb_hid_desc_config_set hid_cntlr_config_desc =
{
    .config =
    {
        .header =
         {
             .bLength         = USB_CFG_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_CONFIG
         },
        .wTotalLength         = HID_CNTLR_CONFIG_DESC_LEN,
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x00U,
        .bmAttributes         = 0xA0U,
        .bMaxPower            = 0x32U
    },

    // interface descriptor
    .hid_itf =
    {
        .header =
         {
             .bLength         = USB_ITF_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_ITF
         },
        .bInterfaceNumber     = HID_CNTLR_INF,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x01U,
        .bInterfaceClass      = USB_HID_CLASS,
        .bInterfaceSubClass   = USB_HID_SUBCLASS_BOOT_ITF,
        .bInterfaceProtocol   = USB_HID_PROTOCOL_CNTLR,
        .iInterface           = 0x00U
    },

    .hid_vendor =
    {
        .header =
         {
             .bLength         = USB_HID_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_HID
         },
        .bcdHID               = 0x0111U,
        .bCountryCode         = 0x00U,
        .bNumDescriptors      = 0x01U,
        .bDescriptorType      = USB_DESCTYPE_REPORT,
        .wDescriptorLength    = HID_CNTLR_REPORT_DESC_LEN,
    },

    .hid_epin =
    {
        .header =
         {
             .bLength         = USB_EP_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_EP
         },
        .bEndpointAddress     = HID_CNTLR_IN_EP,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = HID_CNTLR_IN_PACKET,
        .bInterval            = 0x01U
    },

    .hid_epout =
    {
        .header =
         {
             .bLength         = USB_EP_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_EP
         },
        .bEndpointAddress     = HID_CNTLR_OUT_EP,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = HID_CNTLR_OUT_PACKET,
        .bInterval            = 0x01U
    },
};

//  USB language ID Descriptor
const usb_desc_LANGID usbd_cntlr_language_id_desc =
{
    .header = 
     {
         .bLength         = sizeof(usb_desc_LANGID), 
         .bDescriptorType = USB_DESCTYPE_STR
     },
    .wLANGID              = ENG_LANGID
};

// USB manufacture string
static const usb_desc_str manufacturer_string =
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(10U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'i', 'g', 'a', 'D', 'e', 'v', 'i', 'c', 'e'}
};

// USB product string
static const usb_desc_str product_string =
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(14U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', '-','U', 'S', 'B', '_', 'M', 'o', 'u', 's', 'e'}
};

// USBD serial string
static usb_desc_str serial_string =
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(12U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     }
};

void *const usbd_hid_cntlr_strings[] =
{
    [STR_IDX_LANGID]  = (uint8_t *)&usbd_cntlr_language_id_desc,
    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string
};

usb_desc hid_cntlr_desc = {
    .dev_desc    = (uint8_t *)&hid_cntlr_dev_desc,
    .config_desc = (uint8_t *)&hid_cntlr_config_desc,
    .strings     = usbd_hid_cntlr_strings
};


const uint8_t hid_cntlr_report_desc[HID_CNTLR_REPORT_DESC_LEN] =
{
    USAGE_PAGE, USAGE_PAGE_GENERIC_DESKTOP,
        USAGE, USAGE_GENERIC_DESKTOP_PAGE_GAMEPAD,
        COLLECTION, COLLECTION_APPLICATION,

            COLLECTION, COLLECTION_PHYSICAL,
            USAGE_PAGE, USAGE_PAGE_BUTTON,
            USAGE_MINIMUM, USAGE_BUTTON_PAGE_1,
            USAGE_MAXIMUM, USAGE_BUTTON_PAGE_16,
            LOGICAL_MINIMUM, 0x00,
            LOGICAL_MAXIMUM, 0x01,
            REPORT_SIZE, 0x01,
            REPORT_COUNT, 0x10,
            USAGE_TYPE_INPUT, USAGE_TYPE_DATA_DV,

            USAGE_PAGE, USAGE_PAGE_GENERIC_DESKTOP,
            USAGE, USAGE_GENERIC_DESKTOP_PAGE_X,
            USAGE, USAGE_GENERIC_DESKTOP_PAGE_Y,
            USAGE, USAGE_GENERIC_DESKTOP_PAGE_RX,
            USAGE, USAGE_GENERIC_DESKTOP_PAGE_RY,
            LOGICAL_MINIMUM, 0x81,
            LOGICAL_MAXIMUM, 0x7f,
            REPORT_SIZE, 0x08,
            REPORT_COUNT, 0x04,
            USAGE_TYPE_INPUT, USAGE_TYPE_DATA_DV,
        END_COLECTION,
    END_COLECTION
};

/* local function prototypes ('static') */
static uint8_t hid_cntlr_init    (usb_dev *udev, uint8_t config_index);
static uint8_t hid_cntlr_deinit  (usb_dev *udev, uint8_t config_index);
static uint8_t hid_cntlr_req     (usb_dev *udev, usb_req *req);
static uint8_t hid_cntlr_data_in (usb_dev *udev, uint8_t ep_num);
static uint8_t hid_cntlr_data_out (usb_dev *udev, uint8_t ep_num);

usb_class_core hid_cntlr_cb = {
    .command         = NO_CMD,
    .alter_set       = 0U,

    .init            = hid_cntlr_init,
    .deinit          = hid_cntlr_deinit,
    .req_proc        = hid_cntlr_req,
    .data_in         = hid_cntlr_data_in,
    .data_out        = hid_cntlr_data_out
};

/*!
    \brief      register HID interface operation functions
    \param[in]  udev: pointer to USB device instance
    \param[in]  hid_fop: HID operation functions structure
    \param[out] none
    \retval     USB device operation status
*/
uint8_t hid_cntlr_itfop_register (usb_dev *udev, hid_cntlr_fop_handler *hid_fop)
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
uint8_t hid_cntlr_report_send (usb_dev *udev, uint8_t *report, uint32_t len)
{
    hid_cntlr_handler *hid = (hid_cntlr_handler *)udev->dev.class_data[HID_CNTLR_INF];

    hid->prev_transfer_complete = 0U;

    usbd_ep_send(udev, HID_CNTLR_IN_EP, report, len);

    return USBD_OK;
}

/*!
    \brief      initialize the HID device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_cntlr_init (usb_dev *udev, uint8_t config_index)
{
    static hid_cntlr_handler hid_handler;

    memset((void *)&hid_handler, 0U, sizeof(hid_cntlr_handler));

    /* Initialize the data Tx endpoint */
    usbd_ep_setup (udev, &(hid_cntlr_config_desc.hid_epin));
    usbd_ep_setup (udev, &(hid_cntlr_config_desc.hid_epout));

    usbd_ep_recev (udev, HID_CNTLR_OUT_EP, hid_handler.data_out, HID_CNTLR_OUT_PACKET);

    hid_handler.prev_transfer_complete = 1U;

    udev->dev.class_data[HID_CNTLR_INF] = (void *)&hid_handler;

    if (NULL != udev->dev.user_data) {
        ((hid_cntlr_fop_handler *)udev->dev.user_data)->hid_itf_config();
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
static uint8_t hid_cntlr_deinit (usb_dev *udev, uint8_t config_index)
{
    /* deinitialize HID endpoints */
    usbd_ep_clear(udev, HID_CNTLR_IN_EP);
    usbd_ep_clear(udev, HID_CNTLR_OUT_EP);

    return USBD_OK;
}

/*!
    \brief      handle the HID class-specific requests
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: device class-specific request
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_cntlr_req (usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->dev.transc_in[0];

    hid_cntlr_handler *hid = (hid_cntlr_handler *)udev->dev.class_data[HID_CNTLR_INF];

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
            transc->remain_len = USB_MIN(HID_CNTLR_REPORT_DESC_LEN, req->wLength);
            transc->xfer_buf = (uint8_t *)hid_cntlr_report_desc;

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
static uint8_t hid_cntlr_data_in (usb_dev *udev, uint8_t ep_num)
{
    /*hid_cntlr_handler *hid = (hid_cntlr_handler *)udev->dev.class_data[HID_MICE_INF];

    if (0U != hid->data[2]) {
        hid->data[2] = 0x00U;

        usbd_ep_send(udev, HID_MICE_IN_EP, hid->data, HID_IN_PACKET);
    } else {
        hid->prev_transfer_complete = 1U;
    }*/

    return USBD_OK;
}

static uint8_t hid_cntlr_data_out (usb_dev *udev, uint8_t ep_num) {
    hid_cntlr_handler *hid = (hid_cntlr_handler *)udev->dev.class_data[HID_CNTLR_INF];

//    switch (hid->data_out[2]) {
//        case KB_CTL_KB_OFF:
////            led_effect = 0;
////            leds_off();
//            KB_CTL &= ~KB_CTL_CNTLR;
//            break;
//
//        case KB_CTL_KB_ON:
////            leds_on();
//            KB_CTL |= KB_CTL_CNTLR;
//            break;
//
////        case LED_SPECTRUM:
////            leds_spectrum();
////            led_effect = LED_SPECTRUM;
////            break;
//
//        default:
//            break;
//    }

    usbd_ep_recev(udev, HID_CNTLR_OUT_EP, hid->data_out, HID_CNTLR_OUT_PACKET);

    return USBD_OK;
}