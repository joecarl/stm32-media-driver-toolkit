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


static void InitMario(MDT_ENTITY* mario_ent) {

	MDT_ENTITY_Init(mario_ent);
	mario_ent->speed = 2;
	mario_ent->x = 30;
	mario_ent->y = 106;
	mario_ent->sprites[0] = &mario_spr0;
	mario_ent->sprites[1] = &mario_spr1;
	mario_ent->sprites[2] = &mario_spr2;
	mario_ent->sprites[3] = &mario_spr3;

}


static void DrawPipe(int x, int y, uint16_t height) {

	MDT_DrawBitmap(&mario_pipe_spr0, x, y);

	for (uint16_t i = 0; i < height; i++) {
		MDT_DrawBitmap(&mario_pipe_spr1, x, y + i + 11);
	}

}


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

	uint8_t bgcolor = 0b00100111;

	MDT_ENTITY mario;
	InitMario(&mario);	

	uint32_t time = MDT_GetMs();


	while (1) {

		float time_inc = ((float) (MDT_GetMs() - time)) / 1000.0;
		time = MDT_GetMs();

		if (!MDT_AUDIO_IsPlaying()) {
			PlaySuperMarioTheme();
		}

		//Process input and physics:
		uint8_t right, left;

		if (MDT_USB_INPUT_IsKbdKeyPressed(MDT_KEY_ESCAPE)) return;
		if (MDT_USB_INPUT_IsKbdKeyPressed(MDT_KEY_A)) left = 1; else left = 0;
		if (MDT_USB_INPUT_IsKbdKeyPressed(MDT_KEY_D)) right = 1; else right = 0;
		if (MDT_USB_INPUT_IsKbdKeyPressed(MDT_KEY_SPACEBAR)) MDT_ENTITY_Jump(&mario);
	
		MDT_ENTITY_ProcessControl(&mario, 0, 0, left, right);
		MDT_ENTITY_ProcessPhysics(&mario, time_inc);

		//Draw frame:
		MDT_Clear(bgcolor);

		REPORT_F("%d", (int) mario.y, 5, 15, 0x00);
		REPORT_F("%d", (int) mario.z, 5, 25, 0x00);
		REPORT_LF("Tick: %lu ms", MDT_GetMs(), 5, 5, 0x00);
		REPORT_LF("FPS: %hu", MDT_GetFPS(), 5, 35, 0x00);

		MDT_ENTITY_Draw(&mario);
		for (uint8_t i = 0; i < 18; i++)
			for (uint8_t j = 0; j < 4; j++)
				MDT_DrawBitmap(&mario_ground_spr, i * 17, 133 + j * 17);

		DrawPipe(110, 92, 30);
		DrawPipe(210, 102, 20);
		MDT_DrawBitmap(&logo_spr, 267, 0);
		
		MDT_WaitForVSync();
		MDT_SwapBuffers();

	}

}
