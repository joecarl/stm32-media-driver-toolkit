/**
 * ARCHIVO: "graphics.c"
 * NOMBRE:  Funciones gráficas
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo contiene funciones para dibujar figuras primitivas como 
 *		líneas, círculos, rectángulos, etc.
 */

#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_dma2d.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_tim.h"
#include "drivers/sdram_driver.h"
#include "drivers/vga_driver.h"
#include "libs/graphics.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>



uint8_t usingDMA2D = 0, disableDMA2D = 0;

static GRAPHICS_InitTypeDef graphicsConfig;



bool GRAPHICS_DMA2D_IsAvailable() {
	
	return !usingDMA2D && ! disableDMA2D;

}


static void GRAPHICS_DMA2D_ClearBitmap(BITMAP* bmp, uint8_t color) {

	if (disableDMA2D) {
		//GRAPHICS_ClearBitmap(bmp, color);
		return;
	}

	usingDMA2D = 1;

	LL_DMA2D_DeInit(DMA2D);

	LL_DMA2D_InitTypeDef DMA2D_InitStruct = {
		.Mode = DMA2D_R2M,
		.ColorMode = DMA2D_ARGB8888,
		.OutputGreen = color,//Green_Value;
		.OutputBlue = color,// Blue_Value;
		.OutputRed = color,// Red_Value;
		.OutputAlpha = color,
		.OutputMemoryAddress = (uint32_t)(bmp->buff),
		.LineOffset = 0,
		.NbrOfLines = bmp->height,
		.NbrOfPixelsPerLines = bmp->width / 4,
	};

	LL_DMA2D_Init(DMA2D, &DMA2D_InitStruct);
	LL_DMA2D_Start(DMA2D);

	while ( !LL_DMA2D_IsActiveFlag_TC(DMA2D) || disableDMA2D );

	usingDMA2D = 0;
	
}



static void GRAPHICS_HW_Accel_Init() {

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);

	TIM_HandleTypeDef hTim;
	hTim.Instance = TIM7;
	hTim.Init.Prescaler = 0;
	hTim.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTim.Init.Period = 1;//12
	hTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTim.Init.RepetitionCounter = 0;
	hTim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&hTim);
	LL_TIM_EnableCounter(TIM7);
	LL_TIM_EnableDMAReq_UPDATE(TIM7);

}



void GRAPHICS_Init(GRAPHICS_InitTypeDef* cfg) {

	graphicsConfig = *cfg;

	if (cfg->useHardwareAcceleration) {
		GRAPHICS_HW_Accel_Init();
	}

	if (cfg->useSDRAM) {
		SDRAM_Init();
	}

}


#define DMA_STREAM LL_DMA_STREAM_2
#define DMA_CHANNEL LL_DMA_CHANNEL_1

static void GRAPHICS_HW_ClearBitmap(BITMAP* bmp, uint8_t color) {

	LL_DMA_DeInit(DMA1, DMA_STREAM);

	while (LL_DMA_IsEnabledStream(DMA1, DMA_STREAM));

	LL_DMA_InitTypeDef DMA_InitStruct = {
		.Channel = DMA_CHANNEL,
		.PeriphOrM2MSrcAddress = (uint32_t) &color,//srcAddress;
		.MemoryOrM2MDstAddress = (uint32_t) (bmp->buff),//destAddress;
		.Direction = LL_DMA_DIRECTION_MEMORY_TO_MEMORY,
		.NbData = (uint32_t) (bmp->width * bmp->height),//El ancho de la resolucion
		.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
		.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
		.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE,
		.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE,
		.Mode = LL_DMA_MODE_NORMAL,
		.Priority = LL_DMA_PRIORITY_LOW,
		.FIFOMode = LL_DMA_FIFOMODE_ENABLE,
		.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_FULL,
		.MemBurst = LL_DMA_MBURST_INC16,
		.PeriphBurst = LL_DMA_PBURST_SINGLE,
	};

	LL_DMA_Init(DMA1, DMA_STREAM, &DMA_InitStruct);
	LL_DMA_EnableStream(DMA1, DMA_STREAM);
	
	while (!LL_DMA_IsActiveFlag_TC2(DMA1));
	
	//LL_TIM_DisableCounter(TIM7);
	LL_DMA_DisableStream(DMA1, DMA_STREAM);

}


static void GRAPHICS_HW_DrawBitmap(BITMAP* bmpdst, BITMAP* bmpsrc, int x, int y) {

	LL_DMA_InitTypeDef DMA_InitStruct = {
		.Channel = DMA_CHANNEL,
		.Direction = DMA_MEMORY_TO_MEMORY,
		.NbData = bmpsrc->width / 4,//bmp->height;//El ancho de la resolucion
		.PeriphOrM2MSrcIncMode = DMA_PINC_ENABLE,
		.MemoryOrM2MDstIncMode = DMA_MINC_ENABLE,
		.PeriphOrM2MSrcDataSize = DMA_PDATAALIGN_BYTE,
		.MemoryOrM2MDstDataSize = DMA_MDATAALIGN_BYTE,
		.Mode = DMA_NORMAL,
		.Priority = DMA_PRIORITY_LOW,
		.FIFOMode = DMA_FIFOMODE_ENABLE,
		.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL,
		.MemBurst = DMA_MBURST_INC8,
		.PeriphBurst = DMA_PBURST_INC8,
	};

	for(uint16_t j = 0; j < bmpsrc->height; j++)
	{
		DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)(bmpsrc->buff) + j * bmpsrc->width;//srcAddress;
		DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)(bmpdst->buff) + x + (j + y) * bmpdst->width;//destAddress;
		LL_DMA_Init(DMA1, DMA_STREAM, &DMA_InitStruct);
		LL_DMA_EnableStream(DMA1, DMA_STREAM);
		while(!LL_DMA_IsActiveFlag_TC2(DMA1));
	}

}


void GRAPHICS_GetBitmapFromContext(BITMAP* bmp, DRAWING_CONTEXT* ctx) {
	
	bmp->buff = ctx->bkbuff;
	bmp->width = ctx->width;
	bmp->height = ctx->height;

}


void GRAPHICS_DrawBitmap(BITMAP* bmpdst, const BITMAP* bmp, int x, int y) {

	int i, j;

	//Calculate safe bounds

	int start_i = x < 0 ? -x : 0; 
	int end_i = x + bmp->width > bmpdst->width ? bmpdst->width - x : bmp->width;

	int start_j = y < 0 ? -y : 0; 
	int end_j = y + bmp->height > bmpdst->height ? bmpdst->height - y : bmp->height;

	//Please note how this loop wont event execute if the image is out of bounds
	for (i = start_i; i < end_i; i++) {
		for (j = start_j; j < end_j; j++) {
			const uint8_t color = bmp->buff[j * bmp->width + i];
			if (color == 0xC7) continue; //transparent color
			bmpdst->buff[(y + j) * bmpdst->width + x + i] = color;
		}
	}

}


/**
 * Dibuja una pirámide tridimensional.
 * x, y:	las posiciones x, y de la base de la pirámide
 * sides:   número de lados de la base de la piramide
 * angle:	el angulo de giro de la base de la piramide
 * 			(giro con respecto al eje Z)
 * radius:	el radio de la base (distancia centro-vertice)
 * height:	altura de la pirámide
 *
 */
void GRAPHICS_Draw3DPyramid(BITMAP* bmp, int x, int y, int sides, float angle, float radius, float height, float x_angle)
{
	int i;
	float inc = 6.283 / sides;
	float base_factor = cos(x_angle);

	for(i = 0; i < sides; i++)
	{
		GRAPHICS_DrawLine(
			bmp,
			x + radius * cos(angle + i * inc),
			y + radius * base_factor * sin(angle + i * inc),
			x + radius * cos(angle + (i + 1) * inc),
			y + radius * base_factor * sin(angle + (i + 1) * inc),
			0xE8
		);
		GRAPHICS_DrawLine(
			bmp,
			x,
			y - height,
			x + radius * cos(angle + i * inc),
			y + radius * base_factor * sin(angle + i * inc),
			0x27
		);
	}
}


void GRAPHICS_InitBitmap(BITMAP* bmp, uint16_t h, uint16_t w) {

	bmp->height = h;
	bmp->width = w;

	if (graphicsConfig.useSDRAM) {
		
		bmp->buff = (__IO uint8_t*) SDRAM_malloc(h * w * sizeof(uint8_t));

	} else {
		
		bmp->buff = (__IO uint8_t*) malloc(h * w * sizeof(uint8_t));

	}
}


void GRAPHICS_InitContext(DRAWING_CONTEXT* ctx, uint16_t h, uint16_t w) {
	
	BITMAP ctx_bmp;

	//Init context backbuffer
	GRAPHICS_InitBitmap(&ctx_bmp, h, w);
	ctx->bkbuff = ctx_bmp.buff;

	//Init context frontbuffer
	GRAPHICS_InitBitmap(&ctx_bmp, h, w);
	ctx->buff = ctx_bmp.buff;

	ctx->height = h;
	ctx->width = w;

}


/**
 * Limpia la imagen actual a un color especificado
 * @param color el color al que se pinta toda la imagen
 */
void GRAPHICS_ClearBitmap(BITMAP* bmp, uint8_t color) {

	if (graphicsConfig.useHardwareAcceleration) {
		GRAPHICS_DMA2D_ClearBitmap(bmp, color);
		return;
	}

	uint32_t i, j;

	for(i = 0; i < bmp->width; i++)
		for(j = 0; j < bmp->height; j++)
			bmp->buff[j * bmp->width + i] = color;

}


void GRAPHICS_SwapContextBuffers(DRAWING_CONTEXT* ctx) {

	__IO uint8_t* aux;
	aux = ctx->buff;
	ctx->buff = ctx->bkbuff;
	ctx->bkbuff = aux;

}


/**
 * Pinta un pixel en las coordenadas especificadas.
 * Notese que no pasa nada si se especifican coordenadas
 * que están fuera de los márgenes del dibujo.
 * @param x
 * @param y coordenadas en las que se dibuja el pixel
 * @param color color del pixel
 */
void GRAPHICS_PutPixel(BITMAP *bmp, int x, int y, uint8_t color)
{
	if (x >= 0 && x < bmp->width && y >= 0 && y < bmp->height)
		bmp->buff[y * bmp->width + x] = color;
}


/**
 * Pinta un rectángulo sin rellenar
 * @param x 
 * @param y coordenadas x,y de la esquina superior izquierda
 * 			del rectangulo
 * @param width ancho del rectángulo
 * @param height altura del rectángulo
 * @param color color del rectángulo
 */
void GRAPHICS_DrawRectangle(BITMAP* bmp, int x, int y, int width, int height, uint8_t color)
{
	uint32_t i, j;
	for(j = 0; j <= height; j++)
	{
		GRAPHICS_PutPixel(bmp, x, j + y, color);
		GRAPHICS_PutPixel(bmp, x + width, j + y, color);
	}
	for(i = 0; i <= width; i++)
	{
		GRAPHICS_PutPixel(bmp, i + x, y, color);
		GRAPHICS_PutPixel(bmp, i + x, y + height, color);
	}
}


/**
 * Dibuja una circunferencia
 * @param x
 * @param y coordenadas x, y de la circunferencia
 * @param radius radio de la circunferencia
 */
void GRAPHICS_DrawCircle(BITMAP* bmp, float x, float y, float radius, float thickness, uint8_t color) {

	int16_t xi, yi;
	float sq_radius = radius + thickness;

	for (xi = -sq_radius; xi < sq_radius; xi++) {
		for (yi = -sq_radius; yi < sq_radius; yi++) {
			const float r = sqrtf(xi * xi + yi * yi);
			if (fabs(r - radius) <= thickness) {
				GRAPHICS_PutPixel(bmp, x + xi, y + yi, color);
			}
		}
	}

}


/**
 * Dibuja una linea recta
 * @param x0
 * @param y0 coordenadas x, y del primer extremo de la recta
 * @param x1
 * @param y1 coordenadas x, y del segundo extremo de la recta
 * @param color color de la recta
 */
void GRAPHICS_DrawLine(BITMAP* bmp,int x0, int y0, int x1, int y1, uint8_t color)
{

	int x, y;
	int incx = 1, incy = 1;
	float deltaerr;

	if (x1 < x0)
		incx = -1;
	if (y1 < y0)
		incy = -1;

	float deltax = x1 - x0;
	float deltay = y1 - y0;
	float error = 0;
	if (deltax != 0)
		deltaerr = fabs (deltay / deltax);    // Assume deltax != 0 (line is not vertical),
		// note that this division needs to be done in a way that preserves the fractional part
	else
	{
		deltaerr = fabs(y1-y0);
		x1 = x0 + 1;
	}
	y = y0;
	for (x = x0; x != x1; x += incx)
	{
		GRAPHICS_PutPixel(bmp, x, y, color);
		error += deltaerr;
		while (error >= 0.5)
		{
			GRAPHICS_PutPixel(bmp, x, y, color);
			y += incy;
			error --;
		}
	}
}



void DrawBitmap(const BITMAP* bmp, int x, int y) {

	BITMAP ctx_bmp;
	GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	GRAPHICS_DrawBitmap(&ctx_bmp, bmp, x, y);

}


void ClearBitmap(uint8_t color) {

	BITMAP ctx_bmp;
	GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	GRAPHICS_ClearBitmap(&ctx_bmp, color);

}


void Draw3DPyramid(int x, int y, int sides, float angle, float radius, float height, float x_angle) {

	BITMAP ctx_bmp;
	GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	GRAPHICS_Draw3DPyramid(&ctx_bmp, x, y, sides, angle, radius, height, x_angle);

}


void DrawRectangle(int x1, int y1, int width, int height, uint8_t color) {

	BITMAP ctx_bmp;
	GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	GRAPHICS_DrawRectangle(&ctx_bmp, x1, y1, width, height, color);

}


void DrawFullRectangle(int x, int y, int width, int height, uint8_t color) {

	BITMAP ctx_bmp;
	GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	//GRAPHICS_DrawFullRectangle(&ctx_bmp, x, y, width, height, color);

}


void PutPixel(int x, int y, uint8_t color) {
	
	BITMAP ctx_bmp;
	GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	GRAPHICS_PutPixel(&ctx_bmp, x, y, color);

}


void DrawLine(int x0, int y0, int x1, int y1, uint8_t color) {
	
	BITMAP ctx_bmp;
	GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	GRAPHICS_DrawLine(&ctx_bmp, x0, y0, x1, y1, color);

}


void DrawCircle(float x, float y, float radius, float thickness, uint8_t color) {
	
	BITMAP ctx_bmp;
	GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	GRAPHICS_DrawCircle(&ctx_bmp, x, y, radius, thickness, color);

}