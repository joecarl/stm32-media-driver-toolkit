#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

#include <stdint.h>


void SERIAL_Init(void);

int SERIAL_Send(uint8_t data);

int SERIAL_SendString(char* str);

int SERIAL_RemainingData();

uint8_t SERIAL_ReadNextByte();

#endif