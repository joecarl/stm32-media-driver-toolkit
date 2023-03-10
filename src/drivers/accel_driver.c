#include <stm32f4xx.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_spi.h>

/**
 * NOTA: en este driver la conexion de pines del acelerometro esta hardcodeada 
 * para la placa discovery con el chip STM32F407, para otras placas habría que 
 * adaptarlo.
 */

uint8_t mySPI_GetData(uint8_t adress) {

	LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_3);// WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

	adress = 0x80 | adress;

	//while (!SPI_I2S_GetFlagStatus(SPI1, SPI_FLAG_TXE));  //transmit buffer empty? //antes: SPI_I2S_FLAG_TXE
	while (!LL_SPI_IsActiveFlag_TXE(SPI1));
	//SPI_I2S_SendData(SPI1, adress);
	LL_SPI_TransmitData8(SPI1, adress);
	while (!LL_SPI_IsActiveFlag_RXNE(SPI1)); //data received? //antes: SPI_I2S_FLAG_RXNE
	LL_SPI_ReceiveData8(SPI1);	//Clear RXNE bit

	while (!LL_SPI_IsActiveFlag_TXE(SPI1));  //transmit buffer empty?
	LL_SPI_TransmitData8(SPI1, 0x00);	//Dummy byte to generate clock
	while (!LL_SPI_IsActiveFlag_RXNE(SPI1)); //data received?
	LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_3);//WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	return LL_SPI_ReceiveData8(SPI1); //return reveiced data

}


void mySPI_SendData(uint8_t adress, uint8_t data) {

	LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_3);// WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

	while (!LL_SPI_IsActiveFlag_TXE(SPI1));  //transmit buffer empty?
	LL_SPI_TransmitData8(SPI1, adress);
	while (!LL_SPI_IsActiveFlag_RXNE(SPI1)); //data received?
	LL_SPI_ReceiveData8(SPI1);

	while (!LL_SPI_IsActiveFlag_TXE(SPI1));  //transmit buffer empty?
	LL_SPI_TransmitData8(SPI1, data);
	while (!LL_SPI_IsActiveFlag_RXNE(SPI1)); //data received?
	LL_SPI_ReceiveData8(SPI1);

	LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_3);//WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

}


void mySPI_Init(void) {

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	LL_SPI_InitTypeDef SPI_InitStruct;

	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;//SPI_DIRECTION_2LINES;// SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;//SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;//SPI_DATASIZE_8BIT;
	SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
	SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;//SPI_PHASE_2EDGE;
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;//SPI_NSS_SOFT;
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;//SPI_BAUDRATEPRESCALER_2;
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;//SPI_FIRSTBIT_MSB;

	LL_SPI_Init(SPI1, &SPI_InitStruct);


	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE , ENABLE);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN;

	LL_GPIO_InitTypeDef gpioSpi = {
		.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7,
		.Mode = LL_GPIO_MODE_ALTERNATE,
		.OutputType = LL_GPIO_OUTPUT_PUSHPULL,
		.Alternate = LL_GPIO_AF_5, //_SPI1
		.Speed = LL_GPIO_SPEED_FREQ_HIGH,
		.Pull = LL_GPIO_PULL_NO,
	};
	LL_GPIO_Init(GPIOA, &gpioSpi);

	LL_GPIO_InitTypeDef gpioSpiEn = {
		.Pin = LL_GPIO_PIN_3,
		.Mode = LL_GPIO_MODE_OUTPUT,
		.OutputType = LL_GPIO_OUTPUT_PUSHPULL,
		.Speed = LL_GPIO_SPEED_FREQ_HIGH,
		.Pull = LL_GPIO_PULL_UP,
	};
	LL_GPIO_Init(GPIOE, &gpioSpiEn);

	LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_3);//WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	LL_SPI_Enable(SPI1);

}
