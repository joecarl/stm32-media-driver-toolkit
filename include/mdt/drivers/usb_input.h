#ifndef MDT_USB_INPUT_H
#define MDT_USB_INPUT_H

#include <stdbool.h>
#if __has_include( "usbh_hid.h")
#include "usbh_hid.h"
#define USB_SUPPORT_ENABLED
#endif

typedef struct {
	uint8_t state;
	uint8_t lctrl;
	uint8_t lshift;
	uint8_t lalt;
	uint8_t lgui;
	uint8_t rctrl;
	uint8_t rshift;
	uint8_t ralt;
	uint8_t rgui;
	uint8_t keys[6];
} MDT_USB_INPUT_KeybdInfo;

typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t buttons[3];
} MDT_USB_INPUT_MouseInfo;


#ifdef USB_SUPPORT_ENABLED


void MDT_USB_INPUT_Init();

bool MDT_USB_INPUT_IsKbdKeyPressed(uint8_t key);

MDT_USB_INPUT_KeybdInfo* MDT_USB_INPUT_GetKeybdInfo();

MDT_USB_INPUT_MouseInfo* MDT_USB_INPUT_GetMouseInfo();

uint16_t MDT_USB_INPUT_GetEventCount();

USBH_HandleTypeDef* MDT_USB_INPUT_GetHandle();

USBH_StatusTypeDef MDT_USB_INPUT_GetLastCode();

const char* MDT_USB_INPUT_GetState();


#else 


#define MDT_USB_INPUT_Init()

#define MDT_USB_INPUT_GetState() "USB_DISABLED"

#define MDT_USB_INPUT_GetKeybdInfo() (NULL)

#define MDT_USB_INPUT_GetMouseInfo() (NULL)

#define MDT_USB_INPUT_GetEventCount() (0)

#define MDT_USB_INPUT_GetHandle() (NULL)

#define  MDT_USB_INPUT_GetLastCode() (0)

#define MDT_USB_INPUT_IsKbdKeyPressed(k) (false)


#endif

#endif