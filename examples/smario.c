/**
 * ARCHIVO: "smario.c"
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

#include "drivers/serial_driver.h"
#include "drivers/audio_driver.h"
#include "libs/graphics.h"
#include "libs/text.h"
#include "libs/frames.h"
#include "libs/entities.h"

#include "sprites.h"
#include "audio_samples.h"


void mario_demo(void) {

	GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false,
		.mainCtxHeight = 200,
		.mainCtxWidth = 320,
		.videoDriver = VIDEO_DRIVER_VGA,
	};
	GRAPHICS_Init(&graphicsCfg);
	
	AUDIO_Init();

	SERIAL_Init();

	BITMAP main_bmp;

	uint8_t bgcolor = 0b00100111;
	uint8_t no_input = 0;
	uint8_t data = 0;

	Entity mario;
	EntityInit(&mario);
	mario.speed = 2;
	mario.x = 30;
	mario.y = 106;

	while (1) {

		if (!AUDIO_IsPlaying()) {
			PlaySuperMarioTheme();
		}

		//Get input:
		if (SERIAL_RemainingData() > 0) {

			while (SERIAL_RemainingData() > 0)
				data = SERIAL_ReadNextByte();

		} else {

			no_input++;
			if (no_input > 30) {
				data = 0;
				no_input = 0;
			}

		}	

		//Process input and physics:
		uint8_t right, left;
		if (data == 0x03) return; //^C
		if (data == 'a') left = 1; else left = 0;
		if (data == 'd') right = 1; else right = 0;
		if (data == ' ') EntityJump(&mario);

		if (HAL_GetTick() % 2000 == 0) {
			EntityJump(&mario);
		}
		
		EntityProcessControl(&mario, 0, 0, left, right);
		EntityMove(&mario);

		//Draw frame:
		GRAPHICS_GetBitmapFromContext(&main_bmp, &main_ctx);
		ClearBitmap(bgcolor);

		REPORT_F("%d", (int) mario.y, 10, 15, 0x00);
		REPORT_F("%d", (int) mario.z, 10, 25, 0x00);
		REPORT_LF("Tick: %d ms", HAL_GetTick(), 10, 5, 0x00);
		REPORT_LF("FPS: %d", GetFPS(), 10, 35, 0x00);

		DrawMario(&main_bmp, (int)mario.x, (int)(mario.y - mario.z), mario.spr);
		//EntityDraw(&mario);
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
