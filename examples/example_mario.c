/**
 * ARCHIVO: "mario.c"
 * NOMBRE:  SuperMario example
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo implementa un reducido escenario de conocido videojuego
 *		SuperMario, incluyendo un extracto del tema principal.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stm32f4xx.h>

#include "mdt/drivers/usb_input.h"
#include "mdt/drivers/audio_driver.h"
#include "mdt/clkinfo.h"
#include "mdt/graphics.h"
#include "mdt/text.h"
#include "mdt/ext/entity.h"

#include "mdt/examples/assets/audio_samples.h"
#include "mdt/examples/assets/sprites.h"


void MDT_EXAMPLE_mario(void) {

	MDT_GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false,
		.mainCtxHeight = 200,
		.mainCtxWidth = 320,
		.videoDriver = VIDEO_DRIVER_VGA,
	};
	MDT_GRAPHICS_Init(&graphicsCfg);
	
	MDT_AUDIO_Init();

	MDT_USB_INPUT_Init();

	BITMAP main_bmp;

	uint8_t bgcolor = 0b00100111;

	MDT_ENTITY mario;
	MDT_ENTITY_Init(&mario);
	mario.speed = 2;
	mario.x = 30;
	mario.y = 106;

	uint32_t time = MDT_GetMs();


	while (1) {

		float time_inc = ((float) (MDT_GetMs() - time)) / 1000.0;
		time = MDT_GetMs();

		if (!MDT_AUDIO_IsPlaying()) {
			PlaySuperMarioTheme();
		}

		//Process input and physics:
		uint8_t right, left;

		if (MDT_USB_INPUT_IsKbdKeyPressed(KEY_ESCAPE)) return;
		if (MDT_USB_INPUT_IsKbdKeyPressed(KEY_A)) left = 1; else left = 0;
		if (MDT_USB_INPUT_IsKbdKeyPressed(KEY_D)) right = 1; else right = 0;
		if (MDT_USB_INPUT_IsKbdKeyPressed(KEY_SPACEBAR)) MDT_ENTITY_Jump(&mario);
	
		//if (MDT_GetMs() % 2000 == 0) MDT_ENTITY_Jump(&mario);
 
		MDT_ENTITY_ProcessControl(&mario, 0, 0, left, right);
		MDT_ENTITY_ProcessPhysics(&mario, time_inc);

		//Draw frame:
		MDT_GRAPHICS_GetBitmapFromContext(&main_bmp, &main_ctx);
		ClearBitmap(bgcolor);

		REPORT_F("%d", (int) mario.y, 5, 15, 0x00);
		REPORT_F("%d", (int) mario.z, 5, 25, 0x00);
		REPORT_LF("Tick: %lu ms", MDT_GetMs(), 5, 5, 0x00);
		REPORT_LF("FPS: %hu", GetFPS(), 5, 35, 0x00);

		DrawMario(&main_bmp, (int)mario.x, (int)(mario.y - mario.z), mario.spr);
		//MDT_ENTITY_Draw(&mario);
		for (uint8_t i = 0; i < 18; i++)
			for (uint8_t j = 0; j < 4; j++)
				DrawSuelo(&main_bmp, i * 17, 133 + j * 17);

		DrawTuberia(&main_bmp, 110, 92, 30);
		DrawTuberia(&main_bmp, 210, 102, 20);
		DrawLogo(267, 0);
		
		WaitForVSync();
		SwapContextBuffers();

	}

}


/*

Old implementation reminders:
 
int frameid = AddFrame(150, 150, 150, 150, "super mario");
BITMAP* mario_window = &(frames[frameid].canvas);
strcat(console_text, "\niniciando demo super \nmario.");
strcat(console_text, "\ntomando control del \npuerto serie.");
strcat(console_text, "\npulsa intro para \ndevolver el control");

if (data == 'i') frames[frameid].y -= 3;
if (data == 'k') frames[frameid].y += 3;
if (data == 'j') frames[frameid].x -= 3;
if (data == 'l') frames[frameid].x += 3;
if (data == 0x0D) { data = 0; usart_captured = 0; strcat(console_text, "\ncontrol devuelto\n"); }

*/
