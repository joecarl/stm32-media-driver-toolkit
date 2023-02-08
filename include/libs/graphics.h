#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

#define VIDEO_DRIVER_NONE 0
#define VIDEO_DRIVER_VGA 1

typedef struct {
	bool useHardwareAcceleration;
	bool useSDRAM;
	uint8_t videoDriver;
	uint16_t mainCtxWidth;
	uint16_t mainCtxHeight;
} GRAPHICS_InitTypeDef;

typedef struct {
	__IO uint8_t *buff;
	uint16_t width;
	uint16_t height;
} BITMAP;

typedef struct {
	__IO uint8_t *buff, *bkbuff;
	uint16_t width;
	uint16_t height;
} DRAWING_CONTEXT;


void GRAPHICS_Init(GRAPHICS_InitTypeDef* initOpts);

void GRAPHICS_InitContext(DRAWING_CONTEXT* ctx, uint16_t h, uint16_t w);
void GRAPHICS_SwapContextBuffers(DRAWING_CONTEXT* ctx);
void GRAPHICS_GetBitmapFromContext(BITMAP* bmp, DRAWING_CONTEXT* ctx);
//void GRAPHICS_HW_Accel_Init();
//void GRAPHICS_HW_ClearBitmap(BITMAP* bmp, uint8_t color);
//void GRAPHICS_HW_DrawBitmap(BITMAP* bmpdst, BITMAP* bmpsrc, int x, int y);
//void GRAPHICS_DMA2D_ClearBitmap(BITMAP* bmp, uint8_t c);
//bool GRAPHICS_DMA2D_IsAvailable();

void GRAPHICS_InitBitmap(BITMAP* bmp, uint16_t h, uint16_t w);
void GRAPHICS_DrawBitmap(BITMAP* bmpdst, const BITMAP* bmp, int x, int y);
void GRAPHICS_ClearBitmap(BITMAP* bmp, uint8_t color);
void GRAPHICS_Draw3DPyramid(BITMAP* bmp, int x, int y,int sides, float angle, float radius, float height, float x_angle);
void GRAPHICS_DrawRectangle(BITMAP* bmp, int x1, int y1, int width, int height, uint8_t color);
void GRAPHICS_DrawLine(BITMAP* bmp, int x0, int y0, int x1, int y1, uint8_t color);
void GRAPHICS_DrawCircle(BITMAP* bmp, float x, float y, float radius, float thickness, uint8_t color);
void GRAPHICS_PutPixel(BITMAP* bmp, int x, int y, uint8_t color);

void SwapContextBuffers();
void DrawBitmap(const BITMAP* bmp, int x, int y);
void ClearBitmap(uint8_t color);
void Draw3DPyramid(int x, int y, int sides, float angle, float radius, float height, float x_angle);
void DrawRectangle(int x1, int y1, int width, int height, uint8_t color);
void DrawFullRectangle(int x, int y, int width, int height, uint8_t color);
void PutPixel(int x, int y, uint8_t color);
void DrawLine(int x0, int y0, int x1, int y1, uint8_t color);
void DrawCircle(float x, float y, float radius, float thickness, uint8_t color);

extern DRAWING_CONTEXT main_ctx;

#endif