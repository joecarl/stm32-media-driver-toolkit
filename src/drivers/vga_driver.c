/**
 * ARCHIVO: "vga_driver.c"
 * NOMBRE:  Driver VGA para STM32F4xx
 * AUTOR:   José Carlos Hurtado Romero
 *
 * La implementación se hace mediante 4 timers, un DMA y 10 pines GPIO.
 * Concretamente los recursos utilizados son:
 * TIM1:........... para generar la señal PWM de sincronismo horizontal
 * TIM3:........... para generar la señal PWM de sincronismo vertical
 * TIM2:........... para disparar al Timer 8 en el instante oportuno
 * TIM8:........... para actuar como reloj del DMA
 * DMA2_Stream1:... para transferir los datos del frontbuffer al GPIOC
 * GPIOC[7..0]:.... lleva el valor de cada pixel al DAC externo que terminará en las 3 señales analógicas RGB
 *
 *
 * NOTA: a pesar de que este driver esta pensado para funcionar con el uso adicional de un DAC hecho
 *		unicamente con 8 resistencias, también puede funcionar sin el uso del DAC, reduciendo la
 *		combinacion de colores a 8 en lugar de los 256 que se obtienen con el DAC.
 */

#include <stm32f4xx.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_tim.h>
#include <stm32f4xx_ll_dma.h>
#include <stm32f4xx_ll_dma2d.h>
#include <stdbool.h>

#include "drivers/vga_driver.h"
#include "libs/clkinfo.h"


static VGA_RENDER_STATE vga_render_state;

static VGA_MODE vga_mode;

static VGA_InitTypedef vga_config;


static void InitSyncTimers();

static void InitDMATimers();

static void InitDMA();


void VGA_Init(VGA_InitTypedef* config) {

	LL_AHB1_GRP1_EnableClock(RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN);

	vga_config = *config;

	GPIO_InitTypeDef gpio;

	vga_render_state.video_lines_done = 0;
	vga_render_state.screen_lines_done = 0;
	vga_render_state.screen_refresh_count = 0;

	if (config->mode == VGA_640x400) {

		vga_mode.screen_lines = 449;
		vga_mode.video_lines = 400;
		vga_mode.refresh_rate = 70;
		
	} else if (config->mode == VGA_640x480) {
		
		vga_mode.screen_lines = 526;
		vga_mode.video_lines = 480;
		vga_mode.refresh_rate = 60;
		
	}

	//CONFIGURAMOS LOS PINES PA10(sincronismo horizontal) Y PC8(sincronismo vertical)
	
	gpio.Mode = GPIO_MODE_AF_PP; //PWM
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Pull = GPIO_NOPULL; //Sin resistencas pull-up ni pull-down

	gpio.Pin = GPIO_PIN_10;
	gpio.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Pin = GPIO_PIN_0;
	gpio.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &gpio);

	//Habilitamos los 8 primeros bits del puerto C para los colores
	//4 tonos de rojo
	//8 tonos de azul
	//8 tonos de verde
	//256 colores
	gpio.Pin = 0xFF00; //La configuración afecta a los 8 bits menos significativos
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &gpio);

	InitSyncTimers();//Para hsync y vsync
	InitDMATimers();//Para controlar el flujo del DMA
	InitDMA();//Para enviar las señales RGB

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

}


void VGA_DeInit() {

	LL_DMA_DeInit(DMA2, LL_DMA_STREAM_1);
	LL_TIM_DeInit(TIM2);
	LL_TIM_DeInit(TIM8);
	LL_TIM_DeInit(TIM1);
	LL_TIM_DeInit(TIM3);

}


void VGA_WaitForVSync() {

	while (
		vga_render_state.screen_lines_done > 33 &&
		vga_render_state.screen_lines_done < 33 + vga_mode.video_lines
	);

}


/**
 * Inicializa los Timers 1 y 3 para las señales de sincronismo.
 * Téngase en cuenta que cada evento TIM1_Update equivale a un
 * ciclo de reloj para el timer 3.
 */
static void InitSyncTimers() {

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

	TIM_HandleTypeDef hTim1 = {
		.Instance = TIM1,
		.Init = { 
			.Prescaler = 0,
			.CounterMode = TIM_COUNTERMODE_UP,
			.Period = 31.777 * GetAPB2TimersMHz(),
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.RepetitionCounter = 0,
			.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
		}
	};
	HAL_TIM_Base_Init(&hTim1);

	LL_TIM_EnableMasterSlaveMode(TIM1);
	LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_UPDATE);


	//Configuramos el TIM3 para la señal VSYNC, lo haremos utilizando el TIM1 como reloj

	TIM_HandleTypeDef hTim3 = {
		.Instance = TIM3,
		.Init = {
			.Prescaler = 0,
			.CounterMode = TIM_COUNTERMODE_UP,
			.Period = vga_mode.screen_lines,//la duracion de un ciclo entero de vsync es "screen_lines" veces la del hsync
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.RepetitionCounter = 0,
			.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
		}
	};
	HAL_TIM_Base_Init(&hTim3);

	LL_TIM_SetTriggerInput(TIM3, LL_TIM_TS_ITR0);//El TIM1 esta conectado al ITR0 del TIM3
	LL_TIM_SetSlaveMode(TIM3, LL_TIM_SLAVEMODE_GATED);


	//Configuramos la duracion de los pulsos PWM para las señales de HSYNC y VSYNC
	
	LL_TIM_OC_InitTypeDef ocInit = {0,};
	LL_TIM_OC_StructInit(&ocInit);
	ocInit.OCMode = LL_TIM_OCMODE_PWM1;
	ocInit.OCState = LL_TIM_OCSTATE_ENABLE;
	ocInit.OCPolarity = LL_TIM_OCPOLARITY_LOW;
	ocInit.CompareValue = 3.81 * GetAPB2TimersMHz();//h_pulse;
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH3, &ocInit);
	LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH3);

	ocInit.CompareValue = 2;//la duracion del pulso de vsync es 2 veces la del ciclo entero de hsync
	LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH3, &ocInit);
	LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH3);

	LL_TIM_EnableAllOutputs(TIM1);//Habilita la salida PWM//Requerido para los TIMERS 1 y 8

	//Habilitamos los timers 1 y 3
	LL_TIM_EnableCounter(TIM1);
	LL_TIM_EnableCounter(TIM3);

	//Habiltamos las interrupciones de los timers 1 y 3
	LL_TIM_EnableIT_UPDATE(TIM1);
	LL_TIM_EnableIT_UPDATE(TIM3);


	//Especificamos las rutinas de interrupción	

	uint32_t priority = NVIC_EncodePriority(NVIC_PRIORITYGROUP_2, 0, 0);
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, priority);
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

	uint32_t priority2 = NVIC_EncodePriority(NVIC_PRIORITYGROUP_2, 1, 1);
	NVIC_SetPriority(TIM3_IRQn, priority2);
	NVIC_EnableIRQ(TIM3_IRQn);

}


/**
 * Inicializa los Timers 8 y 2 para disparar al DMA en el momento oportuno. 
 * Téngase en cuenta que el TIM2 comienza cada vez que se emite un evento de
 * TIM1_Update y a su vez el TIM2 sirve para disparar al TIM8 siendo este 
 * último el reloj del DMA.
 */
static void InitDMATimers() {

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

	TIM_HandleTypeDef hTim2 = {
		.Instance = TIM2,
		.Init = {
			.Prescaler = 0,
			.CounterMode = TIM_COUNTERMODE_UP,
			.Period = (3.81 + 1.71) * GetAPB1TimersMHz(),
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.RepetitionCounter = 0,
			.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
		}
	};
	HAL_TIM_OnePulse_Init(&hTim2, TIM_OPMODE_SINGLE);
	LL_TIM_EnableMasterSlaveMode(TIM2);//El TIM 2 Es maestro del TIM8
	LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);
	LL_TIM_SetTriggerInput(TIM2, LL_TIM_TS_ITR0);//El TIM1 esta conectado al ITR0 del TIM2
	LL_TIM_SetSlaveMode(TIM2, LL_TIM_SLAVEMODE_TRIGGER);

	TIM_HandleTypeDef hTim8 = {
		.Instance = TIM8,
		.Init = {
			.Prescaler = 0,
			.CounterMode = TIM_COUNTERMODE_UP,
			.Period = (25.42 / vga_config.bufferColumns) * GetAPB2TimersMHz(),//us * MHz = ciclos
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.RepetitionCounter = 0,
			.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
		}
	};
	HAL_TIM_Base_Init(&hTim8);
	LL_TIM_SetTriggerInput(TIM8, LL_TIM_TS_ITR1);//El TIM2 esta conectado al ITR1 del TIM8
	LL_TIM_SetSlaveMode(TIM8, LL_TIM_SLAVEMODE_TRIGGER);

	LL_TIM_EnableDMAReq_UPDATE(TIM8);//El TIM8 sera el reloj del DMA
	//No es necesario habilitar aqui el TIM8 porque esta configurado para que se habilite con el TIM2_UP

}


/**
 * Inicializa el DMA para la transferencia de imagen al periférico de salida
 * que permite generar las señales RGB.
 */
static void InitDMA() {

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);

	while (LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_1));//Wait until its ready to be configured
	LL_DMA_DeInit(DMA2, LL_DMA_STREAM_1);

	LL_DMA_InitTypeDef DMA_InitStruct = {
		.Channel = LL_DMA_CHANNEL_7,
		.PeriphOrM2MSrcAddress = ((uint32_t) &GPIOC->ODR) + 1,
		.MemoryOrM2MDstAddress = (uint32_t) *(vga_config.bufferPointer),
		.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH,
		.NbData = vga_config.bufferColumns,//El ancho de la resolucionBufferSize
		.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
		.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
		.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE,
		.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE,//WORD;
		.Mode = LL_DMA_MODE_NORMAL,//LL_DMA_MODE_CIRCULAR
		.Priority = LL_DMA_PRIORITY_VERYHIGH,
		.FIFOMode = LL_DMA_FIFOMODE_ENABLE,// LL_DMA_FIFOMODE_DISABLE,
		.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_FULL,
		.MemBurst = LL_DMA_MBURST_INC16,// LL_DMA_MBURST_SINGLE,
		.PeriphBurst = LL_DMA_PBURST_SINGLE,
	};

	LL_DMA_Init(DMA2, LL_DMA_STREAM_1, &DMA_InitStruct);
	
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_1);
	
	//Especificamos la rutina de interrupción del DMA
	uint32_t priority = NVIC_EncodePriority(NVIC_PRIORITYGROUP_2, 0, 0);
	NVIC_SetPriority(DMA2_Stream1_IRQn, priority);
	NVIC_EnableIRQ(DMA2_Stream1_IRQn);

}


static inline void SET_DMA_ROW_ADDR(uint16_t r) {

	const uint8_t* const buff = *vga_config.bufferPointer;
	const uint32_t new_mem_addr = (uint32_t) &(buff[(r) * vga_config.bufferColumns]);
	LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_1, new_mem_addr);
	/*
	Disable the DMA request line, then re-enable it to clear any pending request. 
	This will clear any pending request so no bytes will be transmited immediately
	*/
	//TIM8->DIER &= ~ TIM_DIER_UDE;
	//TIM8->DIER |= TIM_DIER_UDE;
	LL_TIM_DisableDMAReq_UPDATE(TIM8);
	LL_TIM_EnableDMAReq_UPDATE(TIM8);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);

}


/**
 * Rutina de interrupción del DMA2_Stream1. Nótese que esta rutina se ejecuta 
 * cada vez que el DMA termina su transferecia. En la rutina se pone a 0 y se
 * deshabilita el Timer de control de flujo del DMA y también se reconfigura 
 * el DMA para la siguiente linea.
 */
void DMA2_Stream1_IRQHandler(void) {

	//Los timers se rehabilitarán con el evento TIM1_Update (es decir, con hsync)
	LL_DMA_ClearFlag_TC1(DMA2);
	LL_TIM_DisableCounter(TIM8);
	TIM8->CNT = 0;
	GPIOC->ODR &= 0x00FF;
	
	int row = vga_render_state.video_lines_done * vga_config.bufferRows / vga_mode.video_lines;
	if (row >= vga_config.bufferRows) {
		return;
	}

	SET_DMA_ROW_ADDR(row);
	
}


/**
 * Rutina de interrupción del timer 1 (SINCRONIZACIÓN HORIZONTAL)
 */
void TIM1_UP_TIM10_IRQHandler() {

	LL_TIM_ClearFlag_UPDATE(TIM1);

	if (vga_render_state.screen_lines_done == 33) {

		/* 
		At this point we will enable DMA transfers for video output, note the 
		DMA will be reconfigured automatically after each transfer in its TC
		interrupt handler.
		*/
		SET_DMA_ROW_ADDR(0);

	} else if (vga_render_state.screen_lines_done > 33) {
		
		if (vga_render_state.video_lines_done < vga_mode.video_lines) {
			vga_render_state.video_lines_done++;
		}

	}

	vga_render_state.screen_lines_done++;

}


/**
 * Rutina de interrupción del timer 3 (SINCRONIZACIÓN VERTICAL)
 */
void TIM3_IRQHandler() {

	LL_TIM_ClearFlag_UPDATE(TIM3);
	
	vga_render_state.v_sync_done = 1;
	vga_render_state.screen_lines_done = 0;
	vga_render_state.video_lines_done = 0;
	vga_render_state.screen_refresh_count++;

}
