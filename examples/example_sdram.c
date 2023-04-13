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
#include <stdarg.h>

#include <stm32f4xx.h>

#include "mdt/drivers/sdram_driver.h"
#include "mdt/graphics.h"
#include "mdt/text.h"

void MDT_Log(const char *format, ...) {
	
	static char logstr[1024];
	char logmsg[64];
	
	va_list argptr;

	va_start(argptr, format);
	vsprintf(logmsg, format, argptr);
	va_end(argptr);

	strcat(logstr, logmsg);
	strcat(logstr, "\n");

	MDT_Clear(0x00);
	MDT_DrawText(logstr, 20, 20, 0xFB);
	MDT_SwapBuffers();

}

void MDT_EXAMPLE_sdram(void) {

	MDT_GRAPHICS_InitTypeDef graphicsCfg = {
		.useHardwareAcceleration = true,
		.useSDRAM = false,
		.mainCtxHeight = 200,
		.mainCtxWidth = 320,
		.videoDriver = VIDEO_DRIVER_VGA,
	};
	MDT_GRAPHICS_Init(&graphicsCfg);

	MDT_Log("SDRAM Test. Will attempt to check 4MB...");
	
	MDT_SDRAM_Init();
	
	uint32_t bytes = 4 * 1024 * 1024;
	uint8_t* base = (uint8_t*) MDT_SDRAM_malloc(bytes * sizeof(uint8_t));
	uint8_t* ptr;
	uint32_t i = 0;

	for (ptr = base; ptr < base + bytes; ptr++) {
		*ptr = i++;
	}
	
	MDT_Log("Memory assigned. Now checking...");

	i = 0;
	
	for (ptr = base; ptr < base + bytes; ptr++) {
		if (*ptr != i++) break;
	}

	MDT_Log("%u / %u checked", ptr - base, bytes);
	MDT_Log(ptr - base < bytes ? "ERROR!" : "SUCCESS!");	

	while (1);

}