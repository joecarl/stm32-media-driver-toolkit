#include <stm32f4xx.h>
#include "stm32f4xx_ll_usart.h"
#include "drivers/serial_driver.h"

uint8_t serial_buffer[512];

int serial_buffer_len = 0;


void SERIAL_Init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	LL_USART_InitTypeDef USART_InitStructure;

	/* enable peripheral clock for USART2 */
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/* GPIOD clock enable */
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	/* GPIOD Configuration:  USART2 TX,RX */
	/* Connect USART2 pins to AF2 */
	// TX,RX = PD5,PD6 o al reves...
	GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.BaudRate = 9600;
	USART_InitStructure.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStructure.StopBits = LL_USART_STOPBITS_1;
	USART_InitStructure.Parity = LL_USART_PARITY_NONE;
	USART_InitStructure.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStructure.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	LL_USART_Init(USART2, &USART_InitStructure);

	LL_USART_EnableIT_RXNE(USART2);// enable the USART receive interrupt

	// Configure the NVIC (nested vector interrupt controller)
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	uint32_t priority = NVIC_EncodePriority(NVIC_PRIORITYGROUP_2, 10, 10);
	NVIC_SetPriority(USART2_IRQn, priority);
	NVIC_EnableIRQ(USART2_IRQn);  

	LL_USART_Enable(USART2);

}


int SERIAL_RemainingData()
{
	return serial_buffer_len;
}


uint8_t SERIAL_ReadNextByte()
{
	uint8_t data = 0;
	int i;

	if (serial_buffer_len > 0) {
		serial_buffer_len--;
		data = serial_buffer[0];
		for (i = 0; i < serial_buffer_len; i++)
			serial_buffer[i] = serial_buffer[i + 1];
	}

	return data;
}


int SERIAL_Send(uint8_t data)
{
	//while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	while (!LL_USART_IsActiveFlag_TXE(USART2));
	
	//USART_SendData(USART2, data);
	LL_USART_TransmitData9(USART2, data);

	return 0;
}


int SERIAL_SendString(char* str)
{
	uint8_t index = 0;
	while (str[index] != '\0')
	{
		while (!LL_USART_IsActiveFlag_TXE(USART2));
		
		LL_USART_TransmitData9(USART2, str[index]);
		
		index++;
	}

	return 0;
}


void USART2_IRQHandler()
{
	//if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	if(LL_USART_IsEnabledIT_RXNE(USART2) && LL_USART_IsActiveFlag_RXNE(USART2))
	{
		//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		LL_USART_ClearFlag_NE(USART2);
		serial_buffer[serial_buffer_len] = LL_USART_ReceiveData9(USART2);
		serial_buffer_len++;
	}
}

