#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

typedef struct {
	__IO uint8_t *buff, *bkbuff;
	uint16_t width;
	uint16_t height;
} DRAWING_CONTEXT;

void CONTEXT_SwapBuffers(DRAWING_CONTEXT* ctx);

typedef struct {
	__IO uint8_t *buff;
	uint16_t width;
	uint16_t height;
} BITMAP;

void BITMAP_GetFromContext(BITMAP* bmp, DRAWING_CONTEXT* ctx);
void BITMAP_HW_Accel_Init();
void BITMAP_HW_ClearImage(BITMAP* bmp, uint8_t color);
void BITMAP_HW_DrawBitmap(BITMAP* bmpdst, BITMAP* bmpsrc, int x, int y);
void BITMAP_DMA2D_ClearImage(BITMAP* bmp, uint8_t c);
bool BITMAP_DMA2D_IsAvailable();

void BITMAP_Init(BITMAP* bmp, uint16_t h, uint16_t w);
void BITMAP_DrawBitmap(BITMAP* bmpdst, const BITMAP* bmp, int x, int y);
void BITMAP_ClearImage(BITMAP* bmp, uint8_t color);
void BITMAP_Draw3DPyramid(BITMAP* bmp, int x, int y,int sides, float angle, float radius, float height, float x_angle);
void BITMAP_DrawRectangle(BITMAP* bmp, int x1, int y1, int width, int height, uint8_t color);
void BITMAP_DrawLine(BITMAP* bmp, int x0, int y0, int x1, int y1, uint8_t color);
void BITMAP_DrawCircle(BITMAP* bmp, float x, float y, float radius, float thickness, uint8_t color);
void BITMAP_PutPixel(BITMAP* bmp, int x, int y, uint8_t color);

void DrawBitmap(const BITMAP* bmp, int x, int y);
void ClearImage(uint8_t color);
void Draw3DPyramid(int x, int y, int sides, float angle, float radius, float height, float x_angle);
void DrawRectangle(int x1, int y1, int width, int height, uint8_t color);
void DrawFullRectangle(int x, int y, int width, int height, uint8_t color);
void PutPixel(int x, int y, uint8_t color);
void DrawLine(int x0, int y0, int x1, int y1, uint8_t color);
void DrawCircle(float x, float y, float radius, float thickness, uint8_t color);

#endif