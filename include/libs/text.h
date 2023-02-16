#ifndef TEXT_H
#define TEXT_H

#include "libs/graphics.h"

#define TEXT_CHAR_WIDTH 6
#define TEXT_LINE_HEIGHT 10


void GRAPHICS_DrawText(BITMAP* bmp, const char* text, int x, int y, uint8_t color);

void DrawText(const char* text, int x, int y,uint8_t color);


#endif