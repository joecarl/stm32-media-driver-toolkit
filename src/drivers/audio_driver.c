/**
 * ARCHIVO:	"audio_driver.c"
 * NOMBRE:	DRIVER DE AUDIO
 * AUTOR:	José Carlos Hurtado
 *
 * 		Implementa una cola de notas que se pueden ir introduciendo una por una.
 * 		Las notas se reproducen al ejecutar la función MDT_AUDIO_Play();
 *
 * 		La onda de salida es cuadrada y se emite por el pin 11 del GPIOE. Está
 *		pensado para conectar un beeper de sistema.
 *
 */

#include <stm32f4xx.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_tim.h>

#include "mdt/drivers/audio_driver.h"
#include "mdt/clkinfo.h"


uint8_t notas_count = 0;
Nota notas[255];
float milliseconds;


/**
 * Elimina la primera nota de la cola y decrementa
 * en 1 la posicion de todas las demás.
 */
static void MDT_AUDIO_PopQueue() {

	for (uint16_t i = 0; i < notas_count; i++)
		notas[i] = notas[i + 1];
	
	notas_count--;

}


void TIM5_IRQHandler() {

	LL_TIM_ClearFlag_UPDATE(TIM5);

	if (notas_count > 0) {
		
		if (notas[0].frec != 0) {
			LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_11);
			milliseconds += 1000.0 / (2.0 * notas[0].frec);
		} else {
			milliseconds = notas[0].time + 1;
		}

		if (milliseconds > notas[0].time) {
			MDT_AUDIO_PopQueue();
			if (notas[0].frec != 0)
				LL_TIM_SetAutoReload(TIM5, MDT_GetAPB1TimersMHz() * 1000000.0 / (2.0 * notas[0].frec));
			else
				LL_TIM_SetAutoReload(TIM5, MDT_GetAPB1TimersMHz() * 1000.0 * notas[0].time);
			milliseconds = 0;
		}

	} else {
		LL_TIM_DisableIT_UPDATE(TIM5);
	}

}


void MDT_AUDIO_AddNote(uint16_t frec, uint16_t time) {

	if (notas_count <= 255) {

		notas[notas_count].frec = frec;
		notas[notas_count].time = time;
		notas_count++;

	}

}


void MDT_AUDIO_Init() {

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
	
	LL_GPIO_InitTypeDef GPIO_InitStruct = {
		.Pin = LL_GPIO_PIN_11,
		.Mode = LL_GPIO_MODE_OUTPUT,
		.OutputType = LL_GPIO_OUTPUT_PUSHPULL,
		.Speed = LL_GPIO_SPEED_FREQ_HIGH,
		.Pull = LL_GPIO_PULL_UP,
	};
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	LL_TIM_InitTypeDef timerInitStruct = {
		.Prescaler = 0,
		.CounterMode = LL_TIM_COUNTERMODE_DOWN,
		.Autoreload = 1000,//Placeholder value
		.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
		.RepetitionCounter = 0,
	};
	LL_TIM_Init(TIM5, &timerInitStruct);
	LL_TIM_EnableCounter(TIM5);

	//Especificamos la rutina de interrupción
	uint32_t priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 3);
	NVIC_SetPriority(TIM5_IRQn, priority);
	NVIC_EnableIRQ(TIM5_IRQn);
	notas_count = 0;

}


void MDT_AUDIO_Play() {

	LL_TIM_SetAutoReload(TIM5, MDT_GetAPB1TimersMHz() * 1000000 / (2 * notas[0].frec));
	LL_TIM_EnableIT_UPDATE(TIM5);

}


uint8_t MDT_AUDIO_IsPlaying() {

	return notas_count;

}
