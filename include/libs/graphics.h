#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>


#define LIGHTRED   0b11000000
#define LIGHTGREEN 0b00111000
#define LIGHTBLUE  0b00000111

#define RED        0b10000000
#define GREEN      0b00100000
#define BLUE       0b00000100

#define DARKRED    0b01000000
#define DARKGREEN  0b00001000
#define DARKBLUE   0b00000001


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

	/**
	 * The number of frames rendered in the last second (this is a valid 
	 * value for retrieving the fps)
	 */
	uint8_t fps;

	/**
	 * Number of frames rendered since the last fps value was calculated. 
	 * This value is used to calculate the final fps value. It is increased 
	 * after each buffer swap and reseted to 0 after each second.
	 */
	uint8_t fps_counter;

	uint32_t prev_ms;

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

uint8_t GetFPS();
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