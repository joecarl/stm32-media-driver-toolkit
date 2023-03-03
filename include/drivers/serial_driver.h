#ifndef MDT_SERIAL_DRIVER_H
#define MDT_SERIAL_DRIVER_H

#include <stdint.h>


void MDT_SERIAL_Init(void);

int MDT_SERIAL_Send(uint8_t data);

int MDT_SERIAL_SendString(char* str);

int MDT_SERIAL_RemainingData();

uint8_t MDT_SERIAL_ReadNextByte();

#endif