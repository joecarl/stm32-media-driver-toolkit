/**
 * ARCHIVO:	"audio_driver.c"
 * NOMBRE:	DRIVER DE AUDIO
 * AUTOR:	José Carlos Hurtado
 *
 * 		Implementa una cola de notas que se pueden ir introduciendo una por una.
 * 		Las notas se reproducen al ejecutar la función AUDIO_Play();
 *
 * 		La onda de salida es cuadrada y se emite por el pin 11 del GPIOE. Está
 *		pensado para conectar un beeper de sistema.
 *
 */

#include <stm32f4xx.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>

#include "drivers/audio_driver.h"
#include "libs/clkinfo.h"


uint8_t notas_count = 0;
Nota notas[255];
float milliseconds;


/**
 * Elimina la primera nota de la cola y decrementa
 * en 1 la posicion de todas las demás.
 */
static void AUDIO_PopQueue()
{
	int i;
	for (i = 0; i < notas_count; i++)
		notas[i] = notas[i+1];

}


void TIM5_IRQHandler() {

	LL_TIM_ClearFlag_UPDATE(TIM5);

	if (notas_count > 0) {
		
		if (notas[0].frec != 0) {
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
			milliseconds += 1000.0 / (2.0 * notas[0].frec);
		} else {
			milliseconds = notas[0].time + 1;
		}

		if (milliseconds > notas[0].time) {
			AUDIO_PopQueue();
			if (notas[0].frec != 0)
				LL_TIM_SetAutoReload(TIM5, GetAPB1TimersMHz() * 1000000.0 / (2.0 * notas[0].frec));
			else
				LL_TIM_SetAutoReload(TIM5, GetAPB1TimersMHz() * 1000.0 * notas[0].time);
			milliseconds = 0;
			notas_count--;
			//LED_Toggle(0);
		}

	} else {
		LL_TIM_DisableIT_UPDATE(TIM5);
	}

}


void AUDIO_AddNote(uint16_t frec, uint16_t time) {

	if (notas_count <= 255) {

		notas[notas_count].frec = frec;
		notas[notas_count].time = time;
		notas_count++;

	}

}


void AUDIO_Init() {

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	LL_TIM_InitTypeDef timerInitStruct;
	timerInitStruct.Prescaler = 0;
	timerInitStruct.CounterMode = TIM_COUNTERMODE_DOWN;
	timerInitStruct.Autoreload = 1000;//Placeholder value
	timerInitStruct.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timerInitStruct.RepetitionCounter = 0;
	LL_TIM_Init(TIM5, &timerInitStruct);
	LL_TIM_EnableCounter(TIM5);

	//Especificamos la rutina de interrupción
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	uint32_t priority = NVIC_EncodePriority(NVIC_PRIORITYGROUP_2, 3, 3);
	NVIC_SetPriority(TIM5_IRQn, priority);
	NVIC_EnableIRQ(TIM5_IRQn);
	notas_count = 0;

}


void AUDIO_Play() {

	LL_TIM_SetAutoReload(TIM5, GetAPB1TimersMHz() * 1000000 / (2 * notas[0].frec));
	LL_TIM_EnableIT_UPDATE(TIM5);

}


uint8_t AUDIO_IsPlaying() {

	return notas_count;

}
