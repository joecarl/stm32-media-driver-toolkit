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
#include "sprites.h"


void DrawStripedCircle(BITMAP* bmp, float x, float y, float radius, float strip_offset, uint8_t color) {

	int16_t xi, yi;
	float sq_radius = radius;

	for (yi = -sq_radius; yi < sq_radius / 2; yi++) {
		if (sin(strip_offset + (float) yi / 1.4) < 0) {
			continue;
		}
		for (xi = -sq_radius; xi < sq_radius; xi++) {
			const float r = sqrtf(xi * xi + yi * yi);
			if (r < radius) {
				GRAPHICS_PutPixel(bmp, x + xi, y + yi, color);
			}
		}
	}

}


void retro_demo(void) {

	GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false
	};
	GRAPHICS_Init(&graphicsCfg);
	VGA_Init_Signal(VGA_320x200);
	AUDIO_Init();

	//char str[50];
	uint16_t tick = 0;
	const float vpx = 150;
	const float vpy = 80;
	const float d = 9.0;

	float z_offset = 0;
	float x_offset = 0;

	BITMAP ctx_bmp;


	while (1) {
		
		GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);

		if (!AUDIO_IsPlaying()) {
			PlayStrangerThings();
		}
		
		ClearBitmap(0x00);
		
		//sprintf(str, "fps: %u", VGA_GetFPS());
		//DrawText(str, 20, 160, 0xC7);
	
		const float y_far = 100;
		const float y_near = 200;

		
		//Calc forward lines projections

		for (uint8_t i = 0; i < 20; i++) {

			const float x_far = i * 20 + sin(6.28 * x_offset) * 50.0;
			const float x_near = x_far + (x_far - vpx) * (y_near - vpy) / (y_far - vpy);
		
			DrawLine(x_near, y_near, x_far, y_far, 0xC7);

		}


		//Draw far line

		DrawLine(0, y_far, 320, y_far, 0xC7);

		
		//Calc horizontal lines projections

		for (uint8_t i = 0; i < 20; i++) {

			const float z = i * 5.0 - 5.0 * z_offset;
			if (z < 0) continue;

			const float y = 200 - ((200 - vpy) / (z + d)) * z;
			if (y < y_far) continue;
		
			DrawLine(0, y, 320, y, 0xC7);

		}

		
		DrawStripedCircle(&ctx_bmp, 150, 80, 40, 6.28 * z_offset, tick % 256);// 0xC7);
		
		const uint16_t m_tick = tick % 500;
		
		//if (m_tick >= 465 && m_tick < 469 || m_tick >= 474 && m_tick < 494 || m_tick >= 498 && m_tick < 500) {
		if (m_tick >= 466 && (tick % 2 == 0 || m_tick >= 482 && m_tick < 492)) {
			DrawCreature(110, 50);
		}
		

		//Increment movement variables

		x_offset += 0.002;

		if (x_offset > 1) {
			x_offset = 0;
		}

		z_offset += 0.04;

		if (z_offset > 1) {
			z_offset = 0;
		}

		tick++;

		VGA_WaitForVSync();
		VGA_SwapBuffers();

	}

}