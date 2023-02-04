#ifndef TEXT_H
#define TEXT_H

#include "stm32f4xx.h"
#include "libs/graphics.h"


void GRAPHICS_DrawText(BITMAP* bmp, const char* text, int x, int y, uint8_t color);

void DrawText(const char* text, int x, int y,uint8_t color);


#endif