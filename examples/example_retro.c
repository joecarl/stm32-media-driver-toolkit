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

#include <stm32f4xx.h>

#include "mdt/drivers/audio_driver.h"
#include "mdt/drivers/sdram_driver.h"
#include "mdt/graphics.h"
#include "mdt/text.h"
#include "mdt/clkinfo.h"

#include "mdt/examples/assets/audio_samples.h"
#include "mdt/examples/assets/sprites.h"


static void DrawStripedCircle(MDT_BITMAP* bmp, float x, float y, float radius, float strip_offset, uint8_t color) {

	int16_t xi, yi;
	float sq_radius = radius;

	for (yi = -sq_radius; yi < sq_radius / 2; yi++) {
		if (sin(strip_offset + (float) yi / 1.4) < 0) {
			continue;
		}
		for (xi = -sq_radius; xi < sq_radius; xi++) {
			const float r = sqrtf(xi * xi + yi * yi);
			if (r < radius) {
				MDT_GRAPHICS_PutPixel(bmp, x + xi, y + yi, color);
			}
		}
	}

}


void MDT_EXAMPLE_retro(void) {

	MDT_GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false,
		.mainCtxWidth = 320,
		.mainCtxHeight = 200,
		.videoDriver = VIDEO_DRIVER_VGA,
	};
	MDT_GRAPHICS_Init(&graphicsCfg);
	MDT_AUDIO_Init();

	uint16_t tick = 0;
	const float vpx = 150;
	const float vpy = 80;
	const float d = 9.0;

	float z_offset = 0;
	float x_offset = 0;

	MDT_BITMAP ctx_bmp;


	while (1) {
		
		MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);

		if (!MDT_AUDIO_IsPlaying()) {
			PlayStrangerThings();
		}
		
		MDT_Clear(0x00);
		
		//REPORT_LF("FPS: %hu", MDT_GetFPS(), 5, 5, 0xFF);
	
		const float y_far = 100;
		const float y_near = 200;

		
		//Calc forward lines projections

		for (uint8_t i = 0; i < 20; i++) {

			const float x_far = i * 20 + sin(6.28 * x_offset) * 50.0;
			const float x_near = x_far + (x_far - vpx) * (y_near - vpy) / (y_far - vpy);
		
			MDT_DrawLine(x_near, y_near, x_far, y_far, 0xC7);

		}


		//Draw far line

		MDT_DrawLine(0, y_far, 320, y_far, 0xC7);

		
		//Calc horizontal lines projections

		for (uint8_t i = 0; i < 20; i++) {

			const float z = i * 5.0 - 5.0 * z_offset;
			if (z < 0) continue;

			const float y = 200 - ((200 - vpy) / (z + d)) * z;
			if (y < y_far) continue;
		
			MDT_DrawLine(0, y, 320, y, 0xC7);

		}

		
		DrawStripedCircle(&ctx_bmp, 150, 80, 40, 6.28 * z_offset, tick % 256);// 0xC7);
		
		const uint16_t m_tick = tick % 500;
		
		//if (m_tick >= 465 && m_tick < 469 || m_tick >= 474 && m_tick < 494 || m_tick >= 498 && m_tick < 500) {
		if (m_tick >= 466 && (tick % 2 == 0 || (m_tick >= 482 && m_tick < 492))) {
			//DrawCreature(110, 50);
			MDT_DrawBitmap(&creature_spr, 110, 50);
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

		MDT_WaitForVSync();
		MDT_SwapBuffers();

	}

}