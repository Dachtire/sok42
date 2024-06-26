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

#include "usbd_hid_mouse.h"
#include "usb_hid.h"
#include "usbd_conf.h"
#include "usbd_enum.h"

#define USBD_VID                     0x28e9U
#define USBD_PID                     0x0389U

/* Note:it should use the C99 standard when compiling the below codes */
/* USB standard device descriptor */
const usb_desc_dev hid_mouse_dev_desc =
{
    .header = 
     {
         .bLength          = USB_DEV_DESC_LEN, 
         .bDescriptorType  = USB_DESCTYPE_DEV
     },
    .bcdUSB                = USB_BCD_RELEASE,
    .bDeviceClass          = 0x00U,
    .bDeviceSubClass       = 0x00U,
    .bDeviceProtocol       = 0x00U,
    .bMaxPacketSize0       = USB_FS_EP0_MAX_LEN,
    .idVendor              = USBD_VID,
    .idProduct             = USBD_PID,
    .bcdDevice             = USB_BCD_DEVICE,
    .iManufacturer         = STR_IDX_MFC,
    .iProduct              = STR_IDX_PRODUCT,
    .iSerialNumber         = STR_IDX_SERIAL,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};

const usb_hid_desc_config_set hid_mouse_config_desc =
{
    .config = 
    {
        .header = 
         {
             .bLength         = USB_CFG_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_CONFIG 
         },
        .wTotalLength         = HID_MOUSE_CONFIG_DESC_LEN,
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x00U,
        .bmAttributes         = HID_BUS_POWERED | HID_REMOTEWAKEUP,
        .bMaxPower            = HID_MAXPOWER(500)
    },

    .itf =
    {
        .header =
         {
             .bLength         = USB_ITF_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_ITF
         },
        .bInterfaceNumber     = USBD_INF_MOUSE,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x02U,
        .bInterfaceClass      = USB_HID_CLASS,
        .bInterfaceSubClass   = USB_HID_SUBCLASS_BOOT_ITF,
        .bInterfaceProtocol   = USB_HID_PROTOCOL_MOUSE,
        .iInterface           = 0x00U
    },

    .hid =
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
        .wDescriptorLength    = HID_MOUSE_REPORT_DESC_SIZE,
    },

    .epin =
    {
        .header =
         {
             .bLength         = USB_EP_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_EP
         },
        .bEndpointAddress     = USBD_EP_IN_MOUSE,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = HID_MOUSE_IN_PACKET,
        .bInterval            = 0x01U
    },

    .epout =
    {
        .header =
         {
             .bLength         = USB_EP_DESC_LEN,
             .bDescriptorType = USB_DESCTYPE_EP
         },
        .bEndpointAddress     = USBD_EP_OUT_MOUSE,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = HID_MOUSE_OUT_PACKET,
        .bInterval            = 0x01U
    },
};

//  USB language ID Descriptor
//const usb_desc_LANGID usbd_mouse_language_id_desc =
//{
//    .header =
//     {
//         .bLength         = sizeof(usb_desc_LANGID),
//         .bDescriptorType = USB_DESCTYPE_STR
//     },
//    .wLANGID              = ENG_LANGID
//};

// USB manufacture string
//const usb_desc_str manufacturer_string =
//{
//    .header =
//     {
//         .bLength         = USB_STRING_LEN(10U),
//         .bDescriptorType = USB_DESCTYPE_STR,
//     },
//    .unicode_string = {'G', 'i', 'g', 'a', 'D', 'e', 'v', 'i', 'c', 'e'}
//};

// USB product string
//const usb_desc_str product_string =
//{
//    .header =
//     {
//         .bLength         = USB_STRING_LEN(14U),
//         .bDescriptorType = USB_DESCTYPE_STR,
//     },
//    .unicode_string = {'G', 'D', '3', '2', '-','U', 'S', 'B', '_', 'M', 'o', 'u', 's', 'e'}
//};

// USBD serial string
//usb_desc_str serial_string =
//{
//    .header =
//     {
//         .bLength         = USB_STRING_LEN(12U),
//         .bDescriptorType = USB_DESCTYPE_STR,
//     }
//};

//void *const usbd_hid_mouse_strings[] =
//{
//    [STR_IDX_LANGID]  = (uint8_t *)&usbd_mouse_language_id_desc,
//    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
//    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
//    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string
//};

//usb_desc hid_mouse_desc = {
//    .dev_desc    = (uint8_t *)&hid_mouse_dev_desc,
//    .config_desc = (uint8_t *)&hid_mouse_config_desc,
//    .strings     = usbd_hid_mouse_strings
//};


const uint8_t usbd_mouse_report_desc[HID_MOUSE_REPORT_DESC_SIZE] =
{
    USAGE_PAGE, USAGE_PAGE_GENERIC_DESKTOP,
    USAGE, USAGE_GENERIC_DESKTOP_PAGE_MOUSE,
    COLLECTION, COLLECTION_APPLICATION,
        USAGE, USAGE_GENERIC_DESKTOP_PAGE_POINTER,
        COLLECTION, COLLECTION_PHYSICAL,
            USAGE_PAGE, USAGE_PAGE_BUTTON,
            USAGE_MINIMUM, USAGE_BUTTON_PAGE_1,
            USAGE_MAXIMUM, 0x50,
            LOGICAL_MINIMUM, 0x00,
            LOGICAL_MAXIMUM, 0x01,
            REPORT_SIZE, 0x01,
            REPORT_COUNT, 0x50,
            USAGE_TYPE_INPUT, USAGE_TYPE_DATA_DV,

            USAGE_PAGE, USAGE_PAGE_GENERIC_DESKTOP,
            USAGE, USAGE_GENERIC_DESKTOP_PAGE_X,
            USAGE, USAGE_GENERIC_DESKTOP_PAGE_Y,
            USAGE, USAGE_GENERIC_DESKTOP_PAGE_WHEEL,
            0x15, 0x81,
            0x25, 0x7f,
            REPORT_SIZE, 0x08,
            REPORT_COUNT, 0x03,
            USAGE_TYPE_INPUT, USAGE_TYPE_DATA_DVR,
        END_COLECTION,

//        0x09, 0x3c,  /* USAGE (Motion Wakeup) */
//        0x05, 0xff,  /* USAGE PAGE (vendor defined) */
//        0x09, 0x01,  /* USAGE(01) */
//        0x15, 0x00,  /* LOGICAL_MINIMUM (0) */
//        0x25, 0x01,  /* LOGICAL_MAXIMUM (1) */
//        0x75, 0x01,  /* REPORT_SIZE (1) */
//        0x95, 0x02,  /* REPORT_COUNT (2) */
//        0xb1, 0x22,  /* Feature (var) */
//        0x75, 0x06,  /* REPORT_SIZE (6) */
//        0x95, 0x01,  /* REPORT_COUNT (1) */
//        0xb1, 0x01,  /* Feature (cnst) */
    END_COLECTION
};

/* local function prototypes ('static') */
//static uint8_t hid_mouse_init    (usb_dev *udev, uint8_t config_index);
//static uint8_t hid_mouse_deinit  (usb_dev *udev, uint8_t config_index);
//static uint8_t hid_mouse_req     (usb_dev *udev, usb_req *req);
//static uint8_t hid_mouse_data_in (usb_dev *udev, uint8_t ep_num);
//static uint8_t hid_mouse_data_out (usb_dev *udev, uint8_t ep_num);

//usb_class_core hid_mouse_cb = {
//    .command         = NO_CMD,
//    .alter_set       = 0U,
//
//    .init            = hid_mouse_init,
//    .deinit          = hid_mouse_deinit,
//    .req_proc        = hid_mouse_req,
//    .data_in         = hid_mouse_data_in,
//    .data_out        = hid_mouse_data_out
//};
//
///*!
//    \brief      register HID interface operation functions
//    \param[in]  udev: pointer to USB device instance
//    \param[in]  hid_fop: HID operation functions structure
//    \param[out] none
//    \retval     USB device operation status
//*/
//uint8_t hid_mouse_itfop_register (usb_dev *udev, hid_mouse_fop_handler *hid_fop)
//{
//    if (NULL != hid_fop) {
//        udev->dev.user_data = (void *)hid_fop;
//
//        return USBD_OK;
//    }
//
//    return USBD_FAIL;
//}
//
///*!
//    \brief      send keyboard report
//    \param[in]  udev: pointer to USB device instance
//    \param[in]  report: pointer to HID report
//    \param[in]  len: data length
//    \param[out] none
//    \retval     USB device operation status
//*/
//uint8_t hid_mouse_report_send (usb_dev *udev, uint8_t *report, uint32_t len)
//{
//    hid_mouse_handler *hid = (hid_mouse_handler *)udev->dev.class_data[USBD_INF_MOUSE];
//
//    hid->prev_transfer_complete = 0U;
//
//    usbd_ep_send(udev, USBD_EP_IN_MOUSE, report, len);
//
//    return USBD_OK;
//}
//
///*!
//    \brief      initialize the HID device
//    \param[in]  udev: pointer to USB device instance
//    \param[in]  config_index: configuration index
//    \param[out] none
//    \retval     USB device operation status
//*/
//static uint8_t hid_mouse_init (usb_dev *udev, uint8_t config_index)
//{
//    static hid_mouse_handler hid_handler;
//
//    memset((void *)&hid_handler, 0U, sizeof(hid_mouse_handler));
//
//    /* Initialize the data Tx endpoint */
//    usbd_ep_setup (udev, &(hid_mouse_config_desc.epin));
//    usbd_ep_setup (udev, &(hid_mouse_config_desc.epout));
//
//    usbd_ep_recev (udev, USBD_EP_OUT_MOUSE, hid_handler.data_out, HID_MOUSE_OUT_PACKET);
//
//    hid_handler.prev_transfer_complete = 1U;
//
//    udev->dev.class_data[USBD_INF_MOUSE] = (void *)&hid_handler;
//
//    if (NULL != udev->dev.user_data) {
//        ((hid_mouse_fop_handler *)udev->dev.user_data)->hid_itf_config();
//    }
//
//    return USBD_OK;
//}
//
///*!
//    \brief      de-initialize the HID device
//    \param[in]  udev: pointer to USB device instance
//    \param[in]  config_index: configuration index
//    \param[out] none
//    \retval     USB device operation status
//*/
//static uint8_t hid_mouse_deinit (usb_dev *udev, uint8_t config_index)
//{
//    /* deinitialize HID endpoints */
//    usbd_ep_clear(udev, USBD_EP_IN_MOUSE);
//
//    return USBD_OK;
//}
//
///*!
//    \brief      handle the HID class-specific requests
//    \param[in]  udev: pointer to USB device instance
//    \param[in]  req: device class-specific request
//    \param[out] none
//    \retval     USB device operation status
//*/
//static uint8_t hid_mouse_req (usb_dev *udev, usb_req *req)
//{
//    usb_transc *transc = &udev->dev.transc_in[0];
//
//    hid_mouse_handler *hid = (hid_mouse_handler *)udev->dev.class_data[USBD_INF_MOUSE];
//
//    switch (req->bRequest) {
//    case GET_REPORT:
//        /* no use for this driver */
//        break;
//
//    case GET_IDLE:
//        transc->xfer_buf = (uint8_t *)&hid->idle_state;
//
//        transc->remain_len = 1U;
//        break;
//
//    case GET_PROTOCOL:
//        transc->xfer_buf = (uint8_t *)&hid->protocol;
//
//        transc->remain_len = 1U;
//        break;
//
//    case SET_REPORT:
//        /* no use for this driver */
//        break;
//
//    case SET_IDLE:
//        hid->idle_state = (uint8_t)(req->wValue >> 8U);
//        break;
//
//    case SET_PROTOCOL:
//        hid->protocol = (uint8_t)(req->wValue);
//        break;
//
//    case USB_GET_DESCRIPTOR:
//        if (USB_DESCTYPE_REPORT == (req->wValue >> 8U)) {
//            transc->remain_len = USB_MIN(HID_MOUSE_REPORT_DESC_SIZE, req->wLength);
//            transc->xfer_buf = (uint8_t *)usbd_mouse_report_desc;
//
//            return REQ_SUPP;
//        }
//        break;
//
//    default:
//        break;
//    }
//
//    return USBD_OK;
//}

/*!
    \brief      handle data stage
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint identifier
    \param[out] none
    \retval     USB device operation status
*/
//static uint8_t hid_mouse_data_in (usb_dev *udev, uint8_t ep_num)
//{
//    /*hid_mouse_handler *hid = (hid_mouse_handler *)udev->dev.class_data[USBD_INF_MOUSE];
//
//    if (0U != hid->data[2]) {
//        hid->data[2] = 0x00U;
//
//        usbd_ep_send(udev, USBD_EP_IN_MOUSE, hid->data, HID_MOUSE_IN_PACKET);
//    } else {
//        hid->prev_transfer_complete = 1U;
//    }*/
//
//    return USBD_OK;
//}
//
//static uint8_t hid_mouse_data_out (usb_dev *udev, uint8_t ep_num) {
//    return USBD_OK;
//}

//void usbd_mouse_report_send(uint8_t *report)
//{
//    UserToPMABufferCopy(report, ENDP3_TXADDR, USBD_REPORT_SIZE_MOUSE);
//    SetEPTxCount(ENDP3, USBD_REPORT_SIZE_MOUSE);
//    SetEPTxValid(ENDP3);
//    usbd_epin_busy[ENDP3] = TRUE;
//}

//uint8_t usbd_mouse_check_send()
//{
//    if (usbd_epin_busy[ENDP3] == FALSE) {
//        return USB_SUCCESS;
//    } else {
//        return USB_ERROR;
//    }
//}