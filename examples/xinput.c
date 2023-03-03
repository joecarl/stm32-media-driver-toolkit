/**
 * ARCHIVO: "xinput.c"
 * NOMBRE:  XInput example
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo implementa un entorno de pruebas para recibir input de
 *		dispositivos USB HID (teclado y ratón)
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stm32f4xx.h>

#include "libs/clkinfo.h"
#include "libs/graphics.h"
#include "libs/text.h"
#include "libs/entity.h"

#include "drivers/usb_input.h"


void DrawPointer(BITMAP *bmp, int x, int y, uint8_t color);

void xinput_demo(void) {

	MDT_GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false,
		.mainCtxHeight = 200,
		.mainCtxWidth = 320,
		.videoDriver = VIDEO_DRIVER_VGA,
	};
	MDT_GRAPHICS_Init(&graphicsCfg);
	
	MDT_USB_INPUT_Init();

	BITMAP main_bmp;

	uint8_t bgcolor = 0b00100100;
	
	int mx = 50;
	int my = 50;
	int mxoffset = 0;
	int myoffset = 0;
	int rx = 150;
	int ry = 120;
	uint16_t rw = 50;
	uint16_t rh = 50;
	bool drag = false;

	USBH_HandleTypeDef *hUsbHost = MDT_USB_INPUT_GetHandle();


	while (1) {

		if (MDT_USB_INPUT_IsKbdKeyPressed(KEY_ESCAPE)) return;
	
		//Draw frame:
		MDT_GRAPHICS_GetBitmapFromContext(&main_bmp, &main_ctx);
		ClearBitmap(bgcolor);

		REPORT_LF("Tick: %lu ms", MDT_GetMs(), 5, 5, 0x00);
		REPORT_LF("FPS: %hu", GetFPS(), 5, 15, 0x00);

		uint16_t event_count = MDT_USB_INPUT_GetEventCount();
		USBH_StatusTypeDef last_code = MDT_USB_INPUT_GetLastCode();
		const char* usb_state = MDT_USB_INPUT_GetState();

		REPORT(hUsbHost->gState, 5, 25, 0xFF);
		REPORT(event_count, 5, 35, 0xFF);
		REPORT(last_code, 5, 45, 0xFF);

		REPORT_F("%s", usb_state, 5, 190, 0xFF);
		
		HID_MOUSE_Info_TypeDef* mouseInfo = MDT_USB_INPUT_GetMouseInfo();
		if (mouseInfo != NULL) {

			REPORT(mouseInfo->x, 140, 5 , 0xBF);
			REPORT(mouseInfo->y, 140, 15 , 0xBF);
			for (uint8_t i = 0; i < 3; i++) {
					REPORT(mouseInfo->buttons[i], 140, 25 + 10 * i, 0xBF);
			}
			mx += (int8_t) mouseInfo->x;
			my += (int8_t) mouseInfo->y;

			mouseInfo->x = 0;
			mouseInfo->y = 0;

			if (mx < 0) mx = 0;
			if (my < 0) my = 0;
			if (mx > main_ctx.width) mx = main_ctx.width;
			if (my > main_ctx.height) my = main_ctx.height;

		}

		HID_KEYBD_Info_TypeDef* kbdInfo = MDT_USB_INPUT_GetKbdInfo();
		if (kbdInfo != NULL) {
			for (uint8_t i = 0; i < 6; i++) {
					REPORT(kbdInfo->keys[i], 140, 65 + 10 * i, 0xF8);
			}
		}

		if (mouseInfo->buttons[0]) {
			if (!drag && mx > rx && mx < rx + rw && my > ry && my < ry + rh) {
				drag = true;
				mxoffset = mx - rx;
				myoffset = my - ry;
			}
		} else {
			drag = false;
		}

		if (drag) {
			rx = mx - mxoffset;
			ry = my - myoffset;
		}

		DrawPointer(&main_bmp, mx, my, 0x00);

		DrawRectangle(rx, ry, rw, rh, drag ? 0xFA : 0x44); 
		
		WaitForVSync();
		SwapContextBuffers();

	}

}

void DrawPointer(BITMAP *bmp, int x, int y, uint8_t color) {
	
	#define PX(_x_, _y_) MDT_GRAPHICS_PutPixel(bmp, x + (_x_), y + (_y_), color);

	PX(0, 0);
	PX(0, 1);PX(1, 1);
	PX(0, 2);PX(1, 2);PX(2, 2);
	PX(0, 3);PX(1, 3);PX(2, 3);PX(3, 3);
	PX(0, 4);PX(1, 4);PX(2, 4);

}


