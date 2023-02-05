/**
 * ARCHIVO: "test_sdram.c"
 * NOMBRE:  Full demo example
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo implementa un programa que escribe y lee datos en la
 *		memoria SDRAM y los muestra en la salida de video
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stm32f4xx.h"
#include "drivers/vga_driver.h"
#include "drivers/sdram_driver.h"
#include "libs/graphics.h"
#include "libs/text.h"

//uint8_t *last_reg = (uint8_t*) 0xD0200000;

//bf = (uint8_t*) 0xD004B000;
//bk = (uint8_t*) 0xD0000000;

#define __SDRAM_BASE (0xD0000000)


void test_sdram(void) {

	uint16_t tick = 0;
	char str[50];
	
	GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false
	};
	GRAPHICS_Init(&graphicsCfg);
	VGA_Init_Signal(VGA_320x200);
	SDRAM_Init();
	
/*
	uint8_t* test_arr = (uint8_t*) SDRAM_malloc(100 * sizeof(uint8_t));
	for(uint8_t i = 0; i < 100; i++) {
		test_arr[i] = i;
	}
*/

	while (1) {

		ClearBitmap(0x00);

		*(__IO uint16_t*) (__SDRAM_BASE) = tick;
		sprintf(str, "TEST: %d", *(__IO uint16_t*) (__SDRAM_BASE));
		DrawText(str, 10, 140, 0xFB);
/*
		for(uint8_t i = 0; i < 10; i++) {
			sprintf(str, "test_arr[%d]: %d", i, test_arr[i]);
			DrawText(str, 180, 140 + i * 10, 0xFB);
		}
*/

		tick++;
		VGA_WaitForVSync();
		VGA_SwapBuffers();

	}

}