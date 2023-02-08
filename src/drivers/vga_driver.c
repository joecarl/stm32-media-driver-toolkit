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

#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_dma2d.h"
#include "drivers/vga_driver.h"
#include "libs/clkinfo.h"
#include <stdbool.h>


static VGA_RENDER_STATE vga_render_state;

static VGA_MODE vga_mode;

static VGA_InitTypedef vga_config;


static void Init_Timers();

static void Init_DMA();

/**
 * Detiene el código que se está ejecutando hasta que se envíe
 * la siguiente señal de sincronismo vertical
 *
 */
void VGA_WaitForVSync()
{

	while (
		vga_render_state.screen_lines_done > 33 &&
		vga_render_state.screen_lines_done < 33 + vga_mode.video_lines
	);

	/*
	vga_render_state.v_sync_done = 0;//Mandamos la peticion de actualización
	while (!vga_render_state.v_sync_done);//Comprobamos si se ha actualizado
	*/
}

/**
 * Returns the fps calculated in the last second
 *
 */
uint8_t VGA_GetFPS()
{
	return vga_render_state.fps;
}




/**
 * Inicializa la señal VGA
 * @param res especifica la resolucion puede tomar los valores siguientes:

 * 		VGA_640x400
 * 		VGA_640x480
 *
 */
void VGA_Init(VGA_InitTypedef* config)
{
	LL_AHB1_GRP1_EnableClock(RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN);

	vga_config = *config;

	GPIO_InitTypeDef gpio;

	vga_render_state.video_lines_done = 0;

	if (config->mode == VGA_640x400 || config->mode == VGA_320x200)
	{
		//main_ctx.width = 640;
		//main_ctx.height = 400;
		vga_mode.screen_lines = 449;
		vga_mode.video_lines = 400;
		vga_mode.refresh_rate = 70;
		/*
		if (config->mode == VGA_320x200)
		{
			main_ctx.width = 320;
			main_ctx.height = 200;
		}
		*/
	}
	else if (config->mode == VGA_640x480 || config->mode == VGA_320x240)
	{
		//main_ctx.width = 640;
		//main_ctx.height = 480;
		vga_mode.screen_lines = 526;
		vga_mode.video_lines = 480;
		vga_mode.refresh_rate = 60;
		/*
		if (config->mode == VGA_320x240)
		{
			main_ctx.width = 320;
			main_ctx.height = 240;
		}
		*/
	}

	//GRAPHICS_InitContext(&main_ctx, main_ctx.height, main_ctx.width);


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

	Init_Timers();//Para hsync y vsync
	Init_DMA();//Para enviar las señales RGB

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

}


/**
 * Inicializa los Timers 1 y 3 para las señales de sincronismo.
 * Téngase en cuenta que cada evento TIM1_Update equivale a un
 * ciclo de reloj para el timer 3.
 */
static void Init_Timers() {

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

	TIM_HandleTypeDef hTim1;
	hTim1.Instance = TIM1;
	hTim1.Init.Prescaler = 0;
	hTim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTim1.Init.Period = 31.777 * GetAPB2TimersMHz();
	hTim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTim1.Init.RepetitionCounter = 0;
	hTim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&hTim1);

	LL_TIM_EnableMasterSlaveMode(TIM1);
	LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_UPDATE);


	//Configuramos el TIM3 para la señal VSYNC, lo haremos utilizando el TIM1 como reloj

	TIM_HandleTypeDef hTim3;
	hTim3.Instance = TIM3;

	hTim3.Init.Prescaler = 0;
	hTim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTim3.Init.Period = vga_mode.screen_lines;//la duracion de un ciclo entero de vsync es "screen_lines" veces la del hsync
	hTim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTim3.Init.RepetitionCounter = 0;
	hTim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&hTim3);

	LL_TIM_SetTriggerInput(TIM3, LL_TIM_TS_ITR0);//El TIM1 esta conectado al ITR0 del TIM3
	LL_TIM_SetSlaveMode(TIM3, LL_TIM_SLAVEMODE_GATED);


	//Configuramos la duracion de los pulsos PWM para las señales de HSYNC y VSYNC
	
	LL_TIM_OC_InitTypeDef outputChannelInit = {0,};
	LL_TIM_OC_StructInit(&outputChannelInit);
	outputChannelInit.OCMode = LL_TIM_OCMODE_PWM1;
	outputChannelInit.OCState = LL_TIM_OCSTATE_ENABLE;
	outputChannelInit.OCPolarity = LL_TIM_OCPOLARITY_LOW;
	outputChannelInit.CompareValue = 3.81 * GetAPB2TimersMHz();//h_pulse;
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH3, &outputChannelInit);
	LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH3);

	outputChannelInit.CompareValue = 2;//la duracion del pulso de vsync es 2 veces la del ciclo entero de hsync
	LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH3, &outputChannelInit);
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
 * Inicializa el DMA para la transferencia de imagen al periférico de salida
 * que permite generar las señales RGB. Se utilizarán los Timers 8 y 2 para 
 * disparar al DMA en el momento oportuno. Téngase en cuenta que el TIM2 
 * comienza cada vez que se emite un evento de TIM1_Update y a su vez el
 * TIM2 sirve para disparar al TIM8 siendo este último el reloj del DMA.
 */
static void Init_DMA()
{

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);


	TIM_HandleTypeDef hTim2;
	hTim2.Instance = TIM2;
	hTim2.Init.Prescaler = 0;
	hTim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTim2.Init.Period = (3.81 + 1.71) * GetAPB1TimersMHz();
	hTim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTim2.Init.RepetitionCounter = 0;
	hTim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&hTim2);
	LL_TIM_EnableMasterSlaveMode(TIM2);//El TIM 2 Es maestro del TIM8
	LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);
	LL_TIM_SetTriggerInput(TIM2, LL_TIM_TS_ITR0);//El TIM1 esta conectado al ITR0 del TIM2
	LL_TIM_SetSlaveMode(TIM2, LL_TIM_SLAVEMODE_TRIGGER);

	
	TIM_HandleTypeDef hTim8;
	hTim8.Instance = TIM8; 
	hTim8.Init.Prescaler = 0;
	hTim8.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTim8.Init.Period = (25.17 / vga_config.bufferColumns) * GetAPB2TimersMHz();//us * MHz = ciclos
	hTim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTim8.Init.RepetitionCounter = 0;
	hTim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&hTim8);
	LL_TIM_SetTriggerInput(TIM8, LL_TIM_TS_ITR1);//El TIM2 esta conectado al ITR1 del TIM8
	LL_TIM_SetSlaveMode(TIM8, LL_TIM_SLAVEMODE_TRIGGER);

	LL_TIM_EnableDMAReq_UPDATE(TIM8);//El TIM8 sera el reloj del DMA
	//No es necesario habilitar aqui el TIM8 porque esta configurado para que se habilite con el TIM2_UP

	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
	LL_DMA_DeInit(DMA2, LL_DMA_STREAM_1);

	while(LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_1));//Wait until its ready to be configured

	LL_DMA_InitTypeDef DMA_InitStruct = {
		.Channel = LL_DMA_CHANNEL_7,
		.PeriphOrM2MSrcAddress = ((uint32_t) &GPIOC->ODR) + 1,
		.MemoryOrM2MDstAddress = (uint32_t) *(vga_config.bufferPointer),//&(main_ctx.buff[0]),
		.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH,
		.NbData = vga_config.bufferColumns,//El ancho de la resolucionBufferSize
		.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
		.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
		.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE,
		.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE,//WORD;
		.Mode = LL_DMA_MODE_NORMAL,//LL_DMA_MODE_CIRCULAR
		.Priority = LL_DMA_PRIORITY_VERYHIGH,
		.FIFOMode = LL_DMA_FIFOMODE_DISABLE,
		.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_FULL,
		.MemBurst = LL_DMA_MBURST_SINGLE,
		.PeriphBurst = LL_DMA_PBURST_SINGLE,
	};

	LL_DMA_Init(DMA2, LL_DMA_STREAM_1, &DMA_InitStruct);
	
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_1);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
	
	//Especificamos la rutina de interrupción del DMA
	uint32_t priority = NVIC_EncodePriority(NVIC_PRIORITYGROUP_2, 0, 0);
	NVIC_SetPriority(DMA2_Stream1_IRQn, priority);
	NVIC_EnableIRQ(DMA2_Stream1_IRQn);


	LL_TIM_EnableIT_UPDATE(TIM2);//Habiltamos las interrupciones del timer2
	//Especificamos las rutinas de interrupción	
	uint32_t priority2 = NVIC_EncodePriority(NVIC_PRIORITYGROUP_2, 0, 0);
	NVIC_SetPriority(TIM2_IRQn, priority2);
	NVIC_EnableIRQ(TIM2_IRQn);

}


bool IsVideoLine() {
	return (0 < vga_render_state.video_lines_done && vga_render_state.video_lines_done < vga_mode.video_lines);
}


void TIM2_IRQHandler(void) {

	LL_TIM_ClearFlag_UPDATE(TIM2);
	LL_TIM_DisableCounter(TIM2);
	TIM2->CNT = 0;
	
	if (IsVideoLine()) {
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
	}

}


/**
 * Rutina de interrupción del DMA2_Stream1. Nótese que esta rutina se ejecuta 
 * cada vez que el DMA termina su transferecia.
 * En la rutina se ponen a 0 y se deshabilitan los Timers de
 * control del DMA y también se reconfigura el DMA para la
 * siguiente linea.
 */
void DMA2_Stream1_IRQHandler(void)
{
	//Los timers se rehabilitarán con el evento TIM1_Update (es decir, con hsync)
	LL_DMA_ClearFlag_TC1(DMA2);
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
	LL_TIM_DisableCounter(TIM8);
	TIM8->CNT = 0;

	if (IsVideoLine()) {

		int row = vga_render_state.video_lines_done * vga_config.bufferRows / vga_mode.video_lines;
		if (row >= vga_config.bufferRows) {
			return;
		}

		//const uint32_t new_mem_addr = (uint32_t) &(main_ctx.buff[row * main_ctx.width]);
		const uint8_t* buff = *vga_config.bufferPointer;
		const uint32_t new_mem_addr = (uint32_t) &(buff[row * vga_config.bufferColumns]);
		LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_1, new_mem_addr);
		//parece que es necesario habilitar aqui el dma, pero luego haya que volver a habilitarlo...
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);

	}
	GPIOC->ODR &= 0x00FF;

}


/**
 * Rutina de interrupción del timer 1
 */
void TIM1_UP_TIM10_IRQHandler()//SINCRONIZACIÓN HORIZONTAL
{
	if (!LL_TIM_IsActiveFlag_UPDATE(TIM1)) {
		return;
	}
	LL_TIM_ClearFlag_UPDATE(TIM1);

	GPIOC->ODR &= 0x00FF;
	if (vga_render_state.screen_lines_done > 33) {
		if (vga_render_state.video_lines_done < vga_mode.video_lines) {
			vga_render_state.video_lines_done++;
		}
	}

	vga_render_state.screen_lines_done++;
}

/**
 * Rutina de interrupción del timer 3
 */
void TIM3_IRQHandler()//SINCRONIZACIÓN VERTICAL
{
	LL_TIM_ClearFlag_UPDATE(TIM3);
	
	GPIOC->ODR &= 0x00FF;
	vga_render_state.v_sync_done = 1;
	vga_render_state.screen_lines_done = 0;
	vga_render_state.video_lines_done = 0;
	vga_render_state.real_fps_counter++;
	
	//Si real_fps_counter >= refresh_rate es que ha pasado un segundo
	if (vga_render_state.real_fps_counter >= vga_mode.refresh_rate)
	{
		vga_render_state.real_fps_counter = 0;
		vga_render_state.fps = vga_render_state.fps_counter;
		vga_render_state.fps_counter = 0;
	}

	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
	const uint32_t new_mem_addr = (uint32_t) *vga_config.bufferPointer;//&(main_ctx.buff[0]);
	LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_1, new_mem_addr);
	//parece que es necesario habilitar aqui el dma, pero luego haya que volver a habilitarlo...
	//LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
	//GPIOC->ODR &= 0x00FF;

}
