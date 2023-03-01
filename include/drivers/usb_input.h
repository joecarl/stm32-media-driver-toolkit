#ifndef USB_INPUT_H
#define USB_INPUT_H

#include <stdbool.h>
#include "usbh_hid.h"


void USB_INPUT_Init();

bool USB_INPUT_IsKbdKeyPressed(uint8_t key);

HID_KEYBD_Info_TypeDef* USB_INPUT_GetKbdInfo();

HID_MOUSE_Info_TypeDef* USB_INPUT_GetMouseInfo();

USBH_HandleTypeDef* USB_INPUT_GetHandle();

uint16_t USB_INPUT_GetEventCount();

USBH_StatusTypeDef USB_INPUT_GetLastCode();

const char* USB_INPUT_GetState();

#endif