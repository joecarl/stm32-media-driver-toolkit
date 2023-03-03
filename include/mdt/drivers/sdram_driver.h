#ifndef MDT_SDRAM_DRIVER_H
#define MDT_SDRAM_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>


void MDT_SDRAM_Init(void);

bool MDT_SDRAM_IsReady();

__IO void* MDT_SDRAM_malloc(size_t size);


#endif
