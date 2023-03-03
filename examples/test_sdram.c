/**
 * ARCHIVO: "test_sdram.c"
 * NOMBRE:  SDRAM R/W example
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo implementa un programa que escribe y lee datos en la
 *		memoria SDRAM y los muestra en la salida de video
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stm32f4xx.h>

#include "mdt/drivers/sdram_driver.h"
#include "mdt/graphics.h"
#include "mdt/text.h"


void test_sdram(void) {

	uint16_t tick = 0;
	char str[50];
	
	MDT_GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false,
		.mainCtxHeight = 200,
		.mainCtxWidth = 320,
		.videoDriver = VIDEO_DRIVER_VGA,
	};
	MDT_GRAPHICS_Init(&graphicsCfg);
	
	MDT_SDRAM_Init();	
	

	uint8_t* test_arr = (uint8_t*) MDT_SDRAM_malloc(100 * sizeof(uint8_t));
	for (uint8_t i = 0; i < 10; i++) {
		test_arr[i] = i;
	}


	while (1) {

		ClearBitmap(0x00);
		
		for (uint8_t i = 0; i < 10; i++) {
			sprintf(str, "test_arr[%d]: %d", i, test_arr[i]);
			DrawText(str, 20, 20 + i * 10, 0xFB);
		}

		tick++;
		WaitForVSync();
		SwapContextBuffers();

	}

}