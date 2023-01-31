#ifndef ACCEL_DRIVER_H
#define ACCEL_DRIVER_H

#include <stdint.h>

void mySPI_Init(void);
uint8_t mySPI_GetData(uint8_t address);
void mySPI_SendData(uint8_t address, uint8_t data);

#endif