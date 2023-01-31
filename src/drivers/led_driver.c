#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"

#include "drivers/led_driver.h"
#define LEDS_GPIO GPIOG

void Init_Leds(void) {

	//1- Estructura de Configuración
	GPIO_InitTypeDef gpio;

	//2 - Habilitación del reloj del periférico
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

	//3 - Relleno de la estructura de configuración
	gpio.Pin = GPIO_PIN_13 | GPIO_PIN_14;//La configuración afecta a los pines 2 y 3
	gpio.Mode = GPIO_MODE_OUTPUT_PP;	//Pines como salidas en modo PushPull
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;	//Velocidad del puerto a 100MHz
	gpio.Pull = GPIO_NOPULL;			//Sin resistencas pull-up ni pull-down

	//4 - Escritura de la configuración en el periférico
	HAL_GPIO_Init(LEDS_GPIO, &gpio); //Se especifica el periférico y un puntero la estructura de configuración

}


void LED_On (uint8_t led){
	switch (led) {
	case 0:									//LEDS 1..2
		HAL_GPIO_WritePin(LEDS_GPIO, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_SET);
		break;
	case 1:									//LED 1
		HAL_GPIO_WritePin(LEDS_GPIO, GPIO_PIN_13, GPIO_PIN_SET);
		break;
	case 2:									//LED 2
		HAL_GPIO_WritePin(LEDS_GPIO, GPIO_PIN_14, GPIO_PIN_SET);
		break;
	
	}
}


void LED_Off (uint8_t led){
	switch (led) {
	case 0:
		HAL_GPIO_WritePin(LEDS_GPIO, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(LEDS_GPIO, GPIO_PIN_13, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(LEDS_GPIO, GPIO_PIN_14, GPIO_PIN_RESET);
		break;
	
	}
}


void LED_Toggle(uint8_t led){

	switch (led){
	case 0:
		HAL_GPIO_TogglePin(LEDS_GPIO, GPIO_PIN_13 | GPIO_PIN_14);
		break;
	case 1:
		HAL_GPIO_TogglePin(LEDS_GPIO, GPIO_PIN_13);
		break;
	case 2:
		HAL_GPIO_TogglePin(LEDS_GPIO, GPIO_PIN_14);
		break;
	}

}


