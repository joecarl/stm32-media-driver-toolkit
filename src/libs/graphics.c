/**
 * ARCHIVO: "graphics.c"
 * NOMBRE:  Funciones gráficas
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo contiene funciones para dibujar figuras primitivas como 
 *		líneas, círculos, rectángulos, etc.
 */

#include <stm32f4xx.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_dma2d.h>
#include <stm32f4xx_ll_dma.h>
#include <stm32f4xx_ll_tim.h>

#include "drivers/sdram_driver.h"
#include "drivers/vga_driver.h"
#include "libs/graphics.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


DRAWING_CONTEXT main_ctx;

uint8_t usingDMA2D = 0, disableDMA2D = 0;

static MDT_GRAPHICS_InitTypeDef graphicsConfig;

static bool graphicsInitialized = false;


bool MDT_GRAPHICS_DMA2D_IsAvailable() {
	
	return !usingDMA2D && ! disableDMA2D;

}


static void MDT_GRAPHICS_DMA2D_ClearBitmap(BITMAP* bmp, uint8_t color) {

	if (disableDMA2D) {
		//MDT_GRAPHICS_ClearBitmap(bmp, color);
		return;
	}

	usingDMA2D = 1;

	LL_DMA2D_DeInit(DMA2D);

	LL_DMA2D_InitTypeDef DMA2D_InitStruct = {
		.Mode = LL_DMA2D_MODE_R2M,
		.ColorMode = LL_DMA2D_OUTPUT_MODE_ARGB8888,
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


static void MDT_GRAPHICS_HW_Accel_Init() {

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	/**
	 * Cuando el DMA trabaja en modo MEMORY TO MEMORY no se utiliza ningún
	 * hardware request para realizar la transferencia, sino que se hace
	 * automáticamente
	 */
}


void MDT_GRAPHICS_DeInit() {

	MDT_VGA_DeInit();
	
	//es probable que, debido a la pequeña memoria disponible, el sistema no
	//sea capaz de reservar nueva memoria si el tamaño del nuevo contexto es 
	//mayor que el del anterior, incluso habiendo liberado la memora
	MDT_GRAPHICS_DestroyContext(&main_ctx);

	graphicsInitialized = false;
	//TODO: improve deinitialization
}


void MDT_GRAPHICS_Init(MDT_GRAPHICS_InitTypeDef* cfg) {

	if (graphicsInitialized) {
		MDT_GRAPHICS_DeInit();
	}

	graphicsInitialized = true;

	graphicsConfig = *cfg;

	if (cfg->useHardwareAcceleration) {
		MDT_GRAPHICS_HW_Accel_Init();
	}

	if (cfg->useSDRAM) {
		MDT_SDRAM_Init();
	}

	MDT_GRAPHICS_InitContext(&main_ctx, cfg->mainCtxHeight, cfg->mainCtxWidth);

	if (cfg->videoDriver == VIDEO_DRIVER_VGA) {

		//Automatically select the best VGA mode for the specified resolution

		uint8_t vga_mode;

		if (cfg->mainCtxHeight <= 200 && cfg->mainCtxWidth <= 320 ) {
			vga_mode = MDT_VGA_640x400;
		} else if (cfg->mainCtxHeight <= 240 && cfg->mainCtxWidth <= 320 ) {
			vga_mode = MDT_VGA_640x480;
		} else if (cfg->mainCtxHeight <= 400 && cfg->mainCtxWidth <= 640 ) {
			vga_mode = MDT_VGA_640x400;
		} else if (cfg->mainCtxHeight <= 480 && cfg->mainCtxWidth <= 640 ) {
			vga_mode = MDT_VGA_640x480;
		} else {
			return;
		}
		
		MDT_VGA_InitTypedef vga_cfg = {
			.mode = vga_mode,
			.bufferPointer = (uint8_t**) &main_ctx.buff,
			.bufferColumns = cfg->mainCtxWidth,
			.bufferRows = cfg->mainCtxHeight,
		};

		MDT_VGA_Init(&vga_cfg);

	}

}


void WaitForVSync() {

	if (graphicsConfig.videoDriver == VIDEO_DRIVER_VGA) {

		MDT_VGA_WaitForVSync();
	
	}

}


#define DMA_STREAM LL_DMA_STREAM_2
#define DMA_CHANNEL LL_DMA_CHANNEL_1

/**
 * NOTE: This approach is NOT working because only DMA2 is capable of making 
 * M2M transfers, but DMA2 is being used for the VGA controller.
 */
static void MDT_GRAPHICS_HW_ClearBitmap(BITMAP* bmp, uint8_t color) {

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

	LL_DMA_ClearFlag_TC2(DMA1);
	//LL_DMA_DisableStream(DMA1, DMA_STREAM);

}


/**
 * NOTE: This approach is NOT working because only DMA2 is capable of making 
 * M2M transfers, but DMA2 is being used for the VGA controller.
 */
static void MDT_GRAPHICS_HW_DrawBitmap(BITMAP* bmpdst, BITMAP* bmpsrc, int x, int y) {

	LL_DMA_InitTypeDef DMA_InitStruct = {
		.Channel = DMA_CHANNEL,
		.Direction = LL_DMA_DIRECTION_MEMORY_TO_MEMORY,
		.NbData = bmpsrc->width / 4,//bmp->height;//El ancho de la resolucion
		.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_INCREMENT,
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

	for (uint16_t j = 0; j < bmpsrc->height; j++) {
		DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)(bmpsrc->buff) + j * bmpsrc->width;//srcAddress;
		DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)(bmpdst->buff) + x + (j + y) * bmpdst->width;//destAddress;
		LL_DMA_Init(DMA1, DMA_STREAM, &DMA_InitStruct);
		LL_DMA_EnableStream(DMA1, DMA_STREAM);
		while (!LL_DMA_IsActiveFlag_TC2(DMA1));
		LL_DMA_ClearFlag_TC2(DMA1);
	}

}


void MDT_GRAPHICS_GetBitmapFromContext(BITMAP* bmp, DRAWING_CONTEXT* ctx) {
	
	bmp->buff = ctx->bkbuff;
	bmp->width = ctx->width;
	bmp->height = ctx->height;

}


void MDT_GRAPHICS_DrawBitmap(BITMAP* bmpdst, const BITMAP* bmp, int x, int y, uint8_t flip) {

	int i, j;

	//Calculate safe bounds

	int start_i = x < 0 ? -x : 0; 
	int end_i = x + bmp->width > bmpdst->width ? bmpdst->width - x : bmp->width;

	int start_j = y < 0 ? -y : 0; 
	int end_j = y + bmp->height > bmpdst->height ? bmpdst->height - y : bmp->height;

	//const int i_offset = flip ? bmp->width - 1 : 0;
	//const int i_mult = flip ? -1 : 1;
	//Please note how this loop wont event execute if the image is out of bounds
	for (i = start_i; i < end_i; i++) {
		const int i_final_offset = i;//i_offset + i * i_mult;
		for (j = start_j; j < end_j; j++) {
			const uint8_t color = bmp->buff[j * bmp->width + i_final_offset];
			if (color == 0xC7) continue; //transparent color
			bmpdst->buff[(y + j) * bmpdst->width + x + i] = color;
		}
	}

}


void MDT_GRAPHICS_Draw3DPyramid(BITMAP* bmp, int x, int y, int sides, float angle, float radius, float height, float x_angle) {

	int i;
	float inc = 6.283 / sides;
	float base_factor = cos(x_angle);

	for (i = 0; i < sides; i++)
	{
		MDT_GRAPHICS_DrawLine(
			bmp,
			x + radius * cos(angle + i * inc),
			y + radius * base_factor * sin(angle + i * inc),
			x + radius * cos(angle + (i + 1) * inc),
			y + radius * base_factor * sin(angle + (i + 1) * inc),
			0xE8
		);
		MDT_GRAPHICS_DrawLine(
			bmp,
			x,
			y - height,
			x + radius * cos(angle + i * inc),
			y + radius * base_factor * sin(angle + i * inc),
			0x27
		);
	}
}


void MDT_GRAPHICS_InitBitmap(BITMAP* bmp, uint16_t h, uint16_t w) {

	bmp->height = h;
	bmp->width = w;

	if (graphicsConfig.useSDRAM) {
		
		bmp->buff = (__IO uint8_t*) MDT_SDRAM_malloc(h * w * sizeof(uint8_t));

	} else {
		
		bmp->buff = (__IO uint8_t*) malloc(h * w * sizeof(uint8_t));

	}
}


void MDT_GRAPHICS_DestroyBitmap(BITMAP* bmp) {

	if (graphicsConfig.useSDRAM) {
		
		//TODO: MDT_SDRAM_free((void*) bmp->buff);

	} else {

		free((void*) bmp->buff);

	}

}


void MDT_GRAPHICS_InitContext(DRAWING_CONTEXT* ctx, uint16_t h, uint16_t w) {
	
	BITMAP ctx_bmp;

	//Init context backbuffer
	MDT_GRAPHICS_InitBitmap(&ctx_bmp, h, w);
	ctx->bkbuff = ctx_bmp.buff;

	//Init context frontbuffer
	MDT_GRAPHICS_InitBitmap(&ctx_bmp, h, w);
	ctx->buff = ctx_bmp.buff;

	ctx->height = h;
	ctx->width = w;

	ctx->fps_counter = 0;
	ctx->fps = 0;
	ctx->prev_ms = 0;

}


void MDT_GRAPHICS_DestroyContext(DRAWING_CONTEXT* ctx) {

	BITMAP ctx_bmp;
	ctx_bmp.height = ctx->height;
	ctx_bmp.width = ctx->width;

	ctx_bmp.buff = ctx->bkbuff;
	MDT_GRAPHICS_DestroyBitmap(&ctx_bmp);
	ctx_bmp.buff = ctx->buff;
	MDT_GRAPHICS_DestroyBitmap(&ctx_bmp);

}


void MDT_GRAPHICS_ClearBitmap(BITMAP* bmp, uint8_t color) {

	if (graphicsConfig.useHardwareAcceleration) {
		//MDT_GRAPHICS_HW_ClearBitmap(bmp, color);
		MDT_GRAPHICS_DMA2D_ClearBitmap(bmp, color);
		return;
	}

	uint32_t i, j;

	for (i = 0; i < bmp->width; i++)
		for (j = 0; j < bmp->height; j++)
			bmp->buff[j * bmp->width + i] = color;

}


void MDT_GRAPHICS_SwapContextBuffers(DRAWING_CONTEXT* ctx) {

	__IO uint8_t* aux;
	aux = ctx->buff;
	ctx->buff = ctx->bkbuff;
	ctx->bkbuff = aux;

	ctx->fps_counter++;
	const uint32_t tick = HAL_GetTick();
	float elapsed_ms = tick - ctx->prev_ms;
	if (elapsed_ms >= 1000.0) {
		ctx->fps = round((float)ctx->fps_counter / (elapsed_ms / 1000.0));
		ctx->fps_counter = 0;
		ctx->prev_ms = tick;
	}

}


void MDT_GRAPHICS_DrawRectangle(BITMAP* bmp, int x, int y, int width, int height, uint8_t color) {

	uint32_t i, j;
	for (j = 0; j <= height; j++)
	{
		MDT_GRAPHICS_PutPixel(bmp, x, j + y, color);
		MDT_GRAPHICS_PutPixel(bmp, x + width, j + y, color);
	}
	for (i = 0; i <= width; i++)
	{
		MDT_GRAPHICS_PutPixel(bmp, i + x, y, color);
		MDT_GRAPHICS_PutPixel(bmp, i + x, y + height, color);
	}

}


void MDT_GRAPHICS_DrawCircle(BITMAP* bmp, float x, float y, float radius, float thickness, uint8_t color) {

	int16_t xi, yi;
	float sq_radius = radius + thickness;

	for (xi = -sq_radius; xi < sq_radius; xi++) {
		uint8_t found_state = 0;
		for (yi = -sq_radius; yi < sq_radius; yi++) {
			const float r = sqrtf(xi * xi + yi * yi);
						
			if (fabs(r - radius) <= thickness) {
				if (found_state == 0) found_state = 1;
				if (found_state == 2) found_state = 3;
				MDT_GRAPHICS_PutPixel(bmp, x + xi, y + yi, color);
			} else if (found_state == 1) {
				yi = -yi;
				found_state = 2;
			} else if (found_state == 3) {
				break;
			}
		}
	}

}


void MDT_GRAPHICS_DrawLine(BITMAP* bmp, int x0, int y0, int x1, int y1, uint8_t color) {

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
		MDT_GRAPHICS_PutPixel(bmp, x, y, color);
		error += deltaerr;
		while (error >= 0.5)
		{
			MDT_GRAPHICS_PutPixel(bmp, x, y, color);
			y += incy;
			error--;
		}
	}
}


void SwapContextBuffers() {

	MDT_GRAPHICS_SwapContextBuffers(&main_ctx);

}


uint8_t GetFPS() {

	return main_ctx.fps;

}


void DrawBitmap(const BITMAP* bmp, int x, int y) {

	BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	MDT_GRAPHICS_DrawBitmap(&ctx_bmp, bmp, x, y, true);

}


void ClearBitmap(uint8_t color) {

	BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	MDT_GRAPHICS_ClearBitmap(&ctx_bmp, color);

}


void Draw3DPyramid(int x, int y, int sides, float angle, float radius, float height, float x_angle) {

	BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	MDT_GRAPHICS_Draw3DPyramid(&ctx_bmp, x, y, sides, angle, radius, height, x_angle);

}


void DrawRectangle(int x1, int y1, int width, int height, uint8_t color) {

	BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	MDT_GRAPHICS_DrawRectangle(&ctx_bmp, x1, y1, width, height, color);

}


void DrawFullRectangle(int x, int y, int width, int height, uint8_t color) {

	BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	//MDT_GRAPHICS_DrawFullRectangle(&ctx_bmp, x, y, width, height, color);

}

/*
void PutPixel(int x, int y, uint8_t color) {
	
	BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	MDT_GRAPHICS_PutPixel(&ctx_bmp, x, y, color);

}
*/

void DrawLine(int x0, int y0, int x1, int y1, uint8_t color) {
	
	BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	MDT_GRAPHICS_DrawLine(&ctx_bmp, x0, y0, x1, y1, color);

}


void DrawCircle(float x, float y, float radius, float thickness, uint8_t color) {
	
	BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	MDT_GRAPHICS_DrawCircle(&ctx_bmp, x, y, radius, thickness, color);

}
