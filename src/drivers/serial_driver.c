#include <stm32f4xx.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_usart.h>

#include "drivers/serial_driver.h"

static uint8_t serial_buffer[512];

static uint16_t serial_buffer_len = 0;


void SERIAL_Init(void) {

	// Enable peripheral clocks

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);


	// GPIOD Configuration:  USART2 TX, RX = PD5, PD6
	
	GPIO_InitTypeDef GPIO_InitStruct = {
		.Pin = GPIO_PIN_5 | GPIO_PIN_6,
		.Mode = GPIO_MODE_AF_PP,
		.Alternate = GPIO_AF7_USART2,
		.Speed = GPIO_SPEED_FREQ_HIGH,
		.Pull = GPIO_PULLUP,
	};
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


	LL_USART_InitTypeDef USART_InitStruct = {
		.BaudRate = 9600,
		.DataWidth = LL_USART_DATAWIDTH_8B,
		.StopBits = LL_USART_STOPBITS_1,
		.Parity = LL_USART_PARITY_NONE,
		.TransferDirection = LL_USART_DIRECTION_TX_RX,
		.HardwareFlowControl = LL_USART_HWCONTROL_NONE,
	};
	LL_USART_Init(USART2, &USART_InitStruct);

	LL_USART_EnableIT_RXNE(USART2);// enable the USART receive interrupt

	// Configure the NVIC (nested vector interrupt controller)
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	uint32_t priority = NVIC_EncodePriority(NVIC_PRIORITYGROUP_2, 10, 10);
	NVIC_SetPriority(USART2_IRQn, priority);
	NVIC_EnableIRQ(USART2_IRQn);  

	LL_USART_Enable(USART2);

}


int SERIAL_RemainingData() {

	return serial_buffer_len;

}


uint8_t SERIAL_ReadNextByte() {

	uint8_t data = 0;
	uint16_t i;

	if (serial_buffer_len > 0) {
		serial_buffer_len--;
		data = serial_buffer[0];
		for (i = 0; i < serial_buffer_len; i++)
			serial_buffer[i] = serial_buffer[i + 1];
	}

	return data;

}


int SERIAL_Send(uint8_t data) {

	while (!LL_USART_IsActiveFlag_TXE(USART2));
	
	LL_USART_TransmitData8(USART2, data);

	return 0;
}


int SERIAL_SendString(char* str) {

	uint8_t index = 0;
	while (str[index] != '\0') {
		
		while (!LL_USART_IsActiveFlag_TXE(USART2));
		
		LL_USART_TransmitData8(USART2, str[index]);
		
		index++;
	}

	return 0;
}


void USART2_IRQHandler() {

	if (LL_USART_IsEnabledIT_RXNE(USART2) && LL_USART_IsActiveFlag_RXNE(USART2)) {

		LL_USART_ClearFlag_NE(USART2);
		serial_buffer[serial_buffer_len] = LL_USART_ReceiveData8(USART2);
		serial_buffer_len++;
	
	}

}

