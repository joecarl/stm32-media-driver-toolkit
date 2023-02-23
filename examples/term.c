/**
 * ARCHIVO: "term.c"
 * NOMBRE:  Terminal example
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo implementa un terminal capaz de recibir input a traves de
 * 		puerto serie y mostrarlo por pantalla
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stm32f4xx.h>

#include "drivers/serial_driver.h"
#include "libs/graphics.h"
#include "libs/text.h"
#include "examples.h"


static void _read();
static void _draw();
static void _process_cmd();

static uint16_t frame = 0;
static GRAPHICS_InitTypeDef graphicsCfg = {
	.useHardwareAcceleration = true,
	.useSDRAM = false,
	.mainCtxHeight = 200,
	.mainCtxWidth = 320,
	.videoDriver = VIDEO_DRIVER_VGA,
};


void term_demo(void) {
	
	GRAPHICS_Init(&graphicsCfg);

	SERIAL_Init();
	
	while (1) {

		ClearBitmap(0x00);
		
		_read();
		_draw();

		frame++;
		WaitForVSync();
		SwapContextBuffers();

	}

}


char console_text[1024] = "";
char cmd[128] = "";
bool newcmd_available = false;

static void _read() {

	char data;

	while (SERIAL_RemainingData() > 0) {

		data = SERIAL_ReadNextByte();

		if (data == 0x1B) {

			char byte1 = SERIAL_ReadNextByte();
			char byte2 = SERIAL_ReadNextByte();

			if (byte1 == 0x5B) {
				if (byte2 == 0x44) { //arrow left
					strcat(console_text, "(LEFT)");
				} else if (byte2 == 0x43) { //arrow left
					strcat(console_text, "(RIGHT)");
				} else if (byte2 == 0x42) { //arrow left
					strcat(console_text, "(DOWN)");
				} else if (byte2 == 0x41) { //arrow left
					strcat(console_text, "(UP)");
				}
			}

		} else if (data == 0x0D) {

			strcat(console_text, "\n");
			strcat(console_text, cmd);

			char str[100];
			sprintf(str, "\r\nComando recibido: \"%s\"\r\n", cmd);
			SERIAL_SendString(str);
			
			_process_cmd();
			
			cmd[0] = '\0';
			newcmd_available = true;

		} else if (data == 0x08) { //backspace

			uint16_t len = strlen(cmd);
			if (len > 0) {
				cmd[len - 1] = '\0';
			}

		} else {

			char str[] = {data, '\0'};
			strcat(cmd, str);

		}

	}
}


typedef struct {
	const char* name;
	void (*fn)();
} program;

static const program programs[] = {
	{
		.name = "mario",
		.fn = mario_demo,
	},
	{
		.name = "retro",
		.fn = retro_demo,
	},
	{
		.name = "test",
		.fn = test_all,
	}
};

static const uint8_t num_prgs = sizeof(programs) / sizeof(program);

/*
NOTE: function pointers syntax is flexible in C:
You can assign by "fn = fn_name" or "fn = &fn_name", you can also call it using
either (*fn)() or fn() directly.
*/

program* get_program(const char* name) {

	for (uint8_t i = 0; i < num_prgs; i++) {
		if (strcmp(name, programs[i].name) == 0) {
			return &programs[i];
		}
	}

	return NULL;

}


static void _process_cmd() {

	program* prg;

	if (strcmp(cmd, "") == 0) {
		//do nothing
	} else if (strcmp(cmd, "clear") == 0) {

		console_text[0] = '\0';

	} else if (strcmp(cmd, "time") == 0) {

		char res[50];
		sprintf(res, "\n%d", HAL_GetTick());
		strcat(console_text, res);

	} else if (strcmp(cmd, "size") == 0) {

		uint32_t w, h;
		GetTextSize(console_text, &w, &h);
		char res[50];
		sprintf(res, "\n%d x %d", w, h);
		strcat(console_text, res);

	} else if (strcmp(cmd, "programs") == 0) {

		for (uint8_t i = 0; i < num_prgs; i++) {
			strcat(console_text, "\n - ");
			strcat(console_text, programs[i].name);
		}

	} else if (prg = get_program(cmd)) {
		
		prg->fn();
		GRAPHICS_Init(&graphicsCfg);

	} else if (strstr(cmd, "hex ") != NULL) {

		const char* str = cmd + 4;
		const len = strlen(str);
		strcat(console_text, "\n0x ");
		for (uint32_t i = 0; i < len; i++) {
			char res[10];
			sprintf(res, "%02X ", str[i]);
			strcat(console_text, res);
		}

	} else {

		strcat(console_text, "\nERROR: comando desconocido");

	}

}


static void _draw() {

	int base_y = main_ctx.height - 15;
	int y = base_y - TEXT_LINE_HEIGHT;

	char* text_ptr = console_text;

	while (*text_ptr) {
		if (*text_ptr == '\n' || *text_ptr == 0x0D)
			y -= TEXT_LINE_HEIGHT;
		text_ptr++;
	}

	DrawText(console_text, 5, y, LIGHTGREEN);
	DrawText(">:", 5, base_y, LIGHTGREEN);
	DrawText(cmd, 5 + TEXT_CHAR_WIDTH * 2, base_y, LIGHTGREEN);
	if (HAL_GetTick() % 400 < 200) {
		DrawText("_", 5 + TEXT_CHAR_WIDTH * (2 + strlen(cmd)), base_y, LIGHTGREEN);
	}

}



/*
	} else if (strcmp(cmd, "mario") == 0) {

		//xTaskCreate(marioTask, "mario", 255, NULL, VGA_TASK_PRIO, NULL);
		mario_demo();
		GRAPHICS_Init(&graphicsCfg);
*/