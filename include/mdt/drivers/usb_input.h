#ifndef MDT_USB_INPUT_H
#define MDT_USB_INPUT_H

#include <stdbool.h>
#include "usbh_hid.h"


void MDT_USB_INPUT_Init();

bool MDT_USB_INPUT_IsKbdKeyPressed(uint8_t key);

HID_KEYBD_Info_TypeDef* MDT_USB_INPUT_GetKbdInfo();

HID_MOUSE_Info_TypeDef* MDT_USB_INPUT_GetMouseInfo();

USBH_HandleTypeDef* MDT_USB_INPUT_GetHandle();

uint16_t MDT_USB_INPUT_GetEventCount();

USBH_StatusTypeDef MDT_USB_INPUT_GetLastCode();

const char* MDT_USB_INPUT_GetState();

#endif