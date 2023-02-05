/**
 * ARCHIVO: "test.c"
 * NOMBRE:  Full demo example
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo implementa un programa que pone de manifiesto la mayoria
 *		de las capacidades soportadas
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stm32f4xx.h"
#include "drivers/vga_driver.h"
#include "drivers/audio_driver.h"
#include "drivers/sdram_driver.h"
#include "libs/graphics.h"
#include "libs/text.h"
#include "libs/clkinfo.h"

#include "audio_samples.h"


uint8_t move = 1;

float speed = 0.25;


void test_all(void) {

	float time = 0, angle = 0;
	uint8_t time_direction = 1;
	char str[50];
	
	GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false
	};
	GRAPHICS_Init(&graphicsCfg);
	VGA_Init_Signal(VGA_320x200);
	AUDIO_Init();


	while (1) {

		if (!AUDIO_IsPlaying()) {
			PlayMarchaImperial();
		}
		
		ClearBitmap(0x00);
		
		sprintf(str, "fps: %u", VGA_GetFPS());
		DrawText(str, 20, 160, 0xFF);
		
		//Pintamos las paletas de colores
		for (uint8_t ck = 0; ck < 4; ck++)
			for (uint8_t ci = 0; ci < 8; ci++)
				for (uint8_t cj = 0; cj < 8; cj++)
					for (uint8_t i = 0; i < 4; i++)
						for (uint8_t j = 0; j < 4; j++)
							PutPixel(
								ci * 4 + i + 10 + 40 * ck, 
								cj * 4 + j + 30, 
								ci + (cj << 3) + (ck << 6)
							);
		
		//for (uint32_t it = 0; it < 1000; it++)
		//	DrawLine(it/10, it/20, it%100, it%200+20, 0xF4);	

		DrawText("Paletas de colores", 10, 10, 0xFF);
		DrawText("abcdefghijklmnñopqrstuvwxyz", 10, 100, 0xFF);
		DrawText("ABCDEFGHIJKLMNÑOPQRSTUVWXYZ", 10, 110, 0xFF);
		DrawText("0123456789", 10, 120, 0xFF);

		sprintf(str, "APB1 Timers freq: %d MHz", (int32_t) GetAPB1TimersMHz());
		DrawText(str, 10, 130, 0xFA);

		Draw3DPyramid(170, 270, 3, angle, 30, 50, 1);
		Draw3DPyramid(190, 420, 6, angle, 30, 100, 1);
		Draw3DPyramid(70, 300, 7, angle, 50, 70, 1);
		Draw3DPyramid(420/2, 130/2, 8, angle, 50, sin(angle) * 80, angle);
		DrawText("Gráficos en 3 dimensiones...", -200 + time, 180, 0xA5);

		float ry = 20 + 0.3 * time;
		DrawCircle(time - 50, ry, 20 + time / 20, 2.0, 0xF6);
		//sprintf(str, "move: %d", move);
		//DrawText(str, 20, 450, 0xFF);
		if (move)
			time += 4 * (time_direction * 2 - 1) * speed;
		angle += 0.12 * speed;

		if(time > 600 || time < 0)
			time_direction ^= 0x01;

		VGA_WaitForVSync();
		VGA_SwapBuffers();

	}

}