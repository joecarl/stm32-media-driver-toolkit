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


uint8_t usart_captured; //deberia ser extern
//char console_text[1000];
char str[100];


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

	usart_captured = 1;

	BITMAP main_bmp;
	BITMAP* mario_window;
	//int frameid = AddFrame(150, 150, 150, 150, "super mario");
	//BITMAP* mario_window = &(frames[frameid].canvas);

	Entity mario;
	EntityInit(&mario);
	int bgcolor = 0b00100111;
	int no_input = 0;
	int i, j;
	mario.speed = 2;
	mario.x = 30;
	mario.y = 106;
	//strcat(console_text, "\niniciando demo super \nmario.");
	//strcat(console_text, "\ntomando control del \npuerto serie.");
	//strcat(console_text, "\npulsa intro para \ndevolver el control");

	char data = 0;

	uint32_t tick = 0;

	while (1) {

		tick++;
		if (tick % 100 == 0) {
			EntityJump(&mario);
		}

		GRAPHICS_GetBitmapFromContext(&main_bmp, &main_ctx);
		mario_window = &main_bmp;
/*
		if (usart_captured && SERIAL_RemainingData() > 0) {

			while (usart_captured && SERIAL_RemainingData() > 0)
				data = SERIAL_ReadNextByte();

		} else {
*/
			no_input++;
			if (no_input > 30) {
				data = 0;
				no_input = 0;
			}

//		}
		

		if (!AUDIO_IsPlaying())
		{
			PlaySuperMarioTheme();
		}

		ClearBitmap(bgcolor);

		sprintf(str, "fps: %u", GetFPS());
		DrawText(str, 20, 20, 0x00);

		uint8_t right, left;
		if (data == 'a') left = 1; else left = 0;
		if (data == 'd') right = 1; else right = 0;
		if (data == ' ') EntityJump(&mario);
		/*
		if (data == 'i') frames[frameid].y -= 3;
		if (data == 'k') frames[frameid].y += 3;
		if (data == 'j') frames[frameid].x -= 3;
		if (data == 'l') frames[frameid].x += 3;
		if (data == 0x0D) { data = 0; usart_captured = 0; strcat(console_text, "\ncontrol devuelto\n"); }
		*/

		EntityProcessControl(&mario, 0, 0, left, right);
		EntityMove(&mario);
		DrawMario(mario_window, (int)mario.x, (int)(mario.y - mario.z), mario.spr);
		//Draw(&mario);
		for (i = 0; i < 18; i++)
			for (j = 0; j < 4; j++)
				DrawSuelo(mario_window, i * 17, 133 + j * 17);

		DrawTuberia(mario_window, 110, 92, 30);
		DrawTuberia(mario_window, 210, 102, 20);
		DrawLogo(267, 0);
		
		WaitForVSync();
		SwapContextBuffers();

		//while (1);

		//vTaskDelay(10);
	}

}
