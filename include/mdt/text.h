#ifndef MDT_TEXT_H
#define MDT_TEXT_H

#include "mdt/graphics.h"

#define MDT_TEXT_CHAR_WIDTH 6
#define MDT_TEXT_LINE_HEIGHT 10

#define REPORT(t, x, y, c) {\
	char str[100];\
	sprintf(str, #t": 0x%X", (t));\
	MDT_DrawText(str, (x), (y), (c));\
} 

#define REPORT_L(l, t, x, y, c) {\
	char str[100];\
	sprintf(str, l": 0x%X", (t));\
	MDT_DrawText(str, (x), (y), (c));\
}

#define REPORT_F(f, t, x, y, c) {\
	char str[100];\
	sprintf(str, #t": "f, (t));\
	MDT_DrawText(str, (x), (y), (c));\
} 

#define REPORT_LF(lf, t, x, y, c) {\
	char str[100];\
	sprintf(str, (lf), (t));\
	MDT_DrawText(str, (x), (y), (c));\
} 


void MDT_GRAPHICS_DrawText(MDT_BITMAP* bmp, const char* text, int x, int y, uint8_t color);


/**
 * Dibuja una cadena de caracteres en la posición especificada
 * @param text el texto a dibujar
 * @param x
 * @param y coordenadas x, y de la esquina superior izquierda
 * 			del cuadro de texto
 * @param color color del texto
 */
void MDT_DrawText(const char* text, int x, int y,uint8_t color);


void GetTextSize(const char* text, uint32_t* width, uint32_t* height);


#endif