/**
 * ARCHIVO: "example_graphics.c"
 * NOMBRE:  Graphics example
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo implementa un programa que pone de manifiesto la mayoria
 *		de las capacidades graficas soportadas
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stm32f4xx.h>

#include "mdt/drivers/audio_driver.h"
#include "mdt/drivers/sdram_driver.h"
#include "mdt/graphics.h"
#include "mdt/text.h"
#include "mdt/clkinfo.h"

#include "mdt/examples/assets/audio_samples.h"


uint8_t move = 1;

float speed = 0.25;


static void DrawColorPalletes(BITMAP* bmp, int x, int y) {

	for (uint8_t ck = 0; ck < 4; ck++)
		for (uint8_t ci = 0; ci < 8; ci++)
			for (uint8_t cj = 0; cj < 8; cj++)
				for (uint8_t i = 0; i < 4; i++)
					for (uint8_t j = 0; j < 4; j++)
						MDT_GRAPHICS_PutPixel(
							bmp,
							x + ci * 4 + i + 40 * ck, 
							y + cj * 4 + j, 
							ci + (cj << 3) + (ck << 6)
						);
}


void MDT_EXAMPLE_graphics(void) {

	float time = 0, angle = 0;
	uint8_t time_direction = 1;
	char str[128];
	
	MDT_GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false,
		.mainCtxHeight = 200,
		.mainCtxWidth = 320,
		.videoDriver = VIDEO_DRIVER_VGA,
	};
	MDT_GRAPHICS_Init(&graphicsCfg);
	MDT_AUDIO_Init();

	BITMAP ctx_bmp;

	while (1) {

		MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);

		if (!MDT_AUDIO_IsPlaying()) {
			PlayMarchaImperial();
		}
		
		ClearBitmap(0x00);
		
		//Pintamos las paletas de colores
		DrawText("Paletas de colores", 10, 10, 0xFF);
		DrawColorPalletes(&ctx_bmp, 10, 30);

		
		sprintf(str, "FPS: %u", GetFPS());
		DrawText(str, 10, 80, 0x1F);

		//for (uint32_t it = 0; it < 1000; it++)
		//	DrawLine(it/10, it/20, it%100, it%200+20, 0xF4);	

		DrawText("abcdefghijklmnñopqrstuvwxyz", 10, 100, 0xFF);
		DrawText("ABCDEFGHIJKLMNÑOPQRSTUVWXYZ", 10, 110, 0xFF);
		DrawText("0123456789", 10, 120, 0xFF);

		sprintf(str, "APB1 Timers freq: %d MHz", (int) MDT_GetAPB1TimersMHz());
		DrawText(str, 10, 130, 0xFA);
		sprintf(str, "APB2 Timers freq: %d MHz", (int) MDT_GetAPB2TimersMHz());
		DrawText(str, 10, 140, 0xFA);
	

		Draw3DPyramid(170, 270, 3, angle, 30, 50, 1);
		Draw3DPyramid(190, 420, 6, angle, 30, 100, 1);
		Draw3DPyramid(250, 180, 5, angle, 15, 30, 1);
		Draw3DPyramid(225, 130/2, 8, angle, 50, sin(angle) * 80, angle);
		DrawText("Gráficos en 3 dimensiones...", -200 + time, 180, 0xA5);

		float ry = 20 + 0.3 * time;
		DrawCircle(time - 50, ry, 20 + time / 20, 1.0, 0xF6);
		//sprintf(str, "move: %d", move);
		//DrawText(str, 20, 450, 0xFF);
		if (move)
			time += 4 * (time_direction * 2 - 1) * speed;
		angle += 0.12 * speed;

		if (time > 600 || time < 0)
			time_direction ^= 0x01;

		WaitForVSync();
		SwapContextBuffers();

	}

}