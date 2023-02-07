/**
 * ARCHIVO: "retro.c"
 * NOMBRE:  Retro demo example
 * AUTOR:   José Carlos Hurtado
 *
 *		Retro synth demo
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


/**
 * Dibuja una circunferencia
 * @param x
 * @param y coordenadas x, y de la circunferencia
 * @param radius radio de la circunferencia
 */
void DrawSynthCircle(float x, float y, float radius, float strip_offset) {

	int16_t xi, yi;
	float sq_radius = radius;

	for (yi = -sq_radius; yi < sq_radius / 2; yi++) {
		if (sin(strip_offset + (float) yi / 1.4) < 0) {
			continue;
		}
		for (xi = -sq_radius; xi < sq_radius; xi++) {
			const float r = sqrtf(xi * xi + yi * yi);
			if (r < radius) {
				PutPixel(x + xi, y + yi, 0xC7);
			}
		}
	}

}


void retro_demo(void) {

	uint8_t move = 1;

	float speed = 0.25;

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

	const float vpx = 150;
	const float vpy = 80;
	const float d = 9.0;

	float z_offset = 0;
	float x_offset = 0;

	while (1) {

		if (!AUDIO_IsPlaying()) {
			PlayMarchaImperial();
		}
		
		ClearBitmap(0x00);
		
		//sprintf(str, "fps: %u", VGA_GetFPS());
		//DrawText(str, 20, 160, 0xFF);
	

		const float y_far = 100;
		const float y_near = 200;

		for (uint8_t i = 0; i < 20; i++) {
			const float x_far = i * 20 + sin(6.28 * x_offset) * 50.0;
			const float x_near = x_far + (x_far - vpx) * (y_near - vpy) / (y_far - vpy);
		
			DrawLine(x_near, y_near, x_far, y_far, 0xFF);
		}

		x_offset += 0.002;

		if (x_offset > 1) {
			x_offset = 0;
		}

		DrawLine(0, y_far, 320, y_far, 0xFF);

		for (uint8_t i = 0; i < 20; i++) {

			const float z = i * 5.0 - 5.0 * z_offset;

			if (z < 0) {
				continue;
			}
			const float y = 200 - ((200 - vpy) / (z + d)) * z;

			if (y < y_far) {
				continue;
			}
		
			DrawLine(0, y, 320, y, 0xFF);
		}

		z_offset += 0.04;

		if (z_offset > 1) {
			z_offset = 0;
		}

		DrawSynthCircle(150, 80, 40, 6.28 * z_offset);
		
		if (move)
			time += 4 * (time_direction * 2 - 1) * speed;
		angle += 0.12 * speed;

		if(time > 600 || time < 0)
			time_direction ^= 0x01;

		VGA_WaitForVSync();
		VGA_SwapBuffers();

	}

}