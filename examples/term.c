/*
#include "stm32f4xx.h"

#include "serial_driver.h"
#include "graphics.h"
#include "sprites.h"
#include "frames.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

uint8_t window_ready = 0, newcmd_available = 0, usart_captured = 0;

int consoleEn = 1;
char console_text[50];
char cmd[100] = "";


void serial_task(void * pvParameters) {

	char data;
	//char i;
	//char str[60];
	//char counter = 0;

	for ( ;; ) {
		//while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
		//{

		//	data = USART_ReceiveData(USART2);
		if (!usart_captured) {

			while(SERIAL_RemainingData() > 0)
			{

				data = SERIAL_ReadNextByte();
				char str[] = {data, '\0'};
				strcat(console_text, str);
				if (data != 0x0D && !newcmd_available)
				{
					strcat(cmd, str);
				}
				else {
					char str[200];
					sprintf(str, "\ncomando recibido: %s\n", cmd);
					SERIAL_SendString(str);
					newcmd_available = 1;
				}

			}

			if (newcmd_available)
			{
				if(strcmp(cmd, "move on") == 0) {

					move = 1;

				} else if(strcmp(cmd, "move off") == 0) {

					move = 0;

				} else if(strcmp(cmd, "exit") == 0) {

					consoleEn = 0;

				} else if (strcmp(cmd, "cls") == 0) {

					//sprintf(console_text, "");
					console_text[0] = '\0';

				} else if (strcmp(cmd, "faster") == 0) {

					speed += 0.3;

				} else if (strcmp(cmd, "slower") == 0) {

					speed -= 0.3;

				} else if (strcmp(cmd, "cmd") == 0) {

					consoleEn = 1;
					//xTaskCreate(console_task, "console", configMINIMAL_STACK_SIZE, NULL, VGA_TASK_PRIO, &xHandle2);

				} else if (strcmp(cmd, "mario") == 0) {

					//xTaskCreate(marioTask, "mario", 255, NULL, VGA_TASK_PRIO, NULL);

				} else {

					strcat(console_text, "comando desconocido\n");

				}

				newcmd_available = 0;
				//sprintf(cmd, "");
				cmd[0] = '\0';
			}

		}

		//sprintf(str, "Escribiendo %d en la posicion %d de SDRAM\n", counter, counter*12034);
		//SERIAL_SendString(str);
		//TM_SDRAM_Write8(counter*12034, counter);
		//sprintf(str, "Leido %d en la posicion %d de SDRAM\n", TM_SDRAM_Read8(counter*12034), counter*12034);
		//SERIAL_SendString(str);
		

		//SERIAL_SendString("Hola");
		//counter++;
		
		//vTaskDelay(200);
	}
}





void console_task(void * pvParameters) {

	int frameid= AddFrame(350, 370, 140, 90, "consola");
	BITMAP* window = &(frames[frameid].canvas);
	//window->bkbuff = test;
	//window->buff = test2;
	window_ready = 1;
	int y;
	float time = 0;

	consoleEn = 1;
	//DMA2D_Suspend(DISABLE);
	LL_DMA2D_Resume(DMA2D);

	sprintf(console_text, "VGA y freertos\nconsola de comandos\n\n");
	while ( consoleEn ) {
		int i;
		y = window->height - 15;
		for (i = 0; i < strlen(console_text); i++)
			if (console_text[i] == '\n' || console_text[i] == 13)
				y -= 10;
		while (usingDMA2D);
		BITMAP_DMA2D_ClearImage(window, 0x10);
		BITMAP_DrawText(window, console_text, 5, y-6*time*move, 0xFF);
		BITMAP_DrawRectangle(window, 0, 0, window->width-1, window->height-1, 0xFF);
		BITMAP_SwapBuffers(window);
		time += 0.3;
		if (time > 20) time = 0;

		//vTaskDelay(100);

	}


	usingDMA2D = 0;

	//vTaskDelete(NULL);

}

*/