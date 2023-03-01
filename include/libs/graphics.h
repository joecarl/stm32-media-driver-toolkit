#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdint.h>


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
void GRAPHICS_DeInit();

void GRAPHICS_InitContext(DRAWING_CONTEXT* ctx, uint16_t h, uint16_t w);
void GRAPHICS_DestroyContext(DRAWING_CONTEXT* ctx);
void GRAPHICS_SwapContextBuffers(DRAWING_CONTEXT* ctx);
void GRAPHICS_GetBitmapFromContext(BITMAP* bmp, DRAWING_CONTEXT* ctx);

void GRAPHICS_InitBitmap(BITMAP* bmp, uint16_t h, uint16_t w);
void GRAPHICS_DestroyBitmap(BITMAP* bmp);
void GRAPHICS_DrawBitmap(BITMAP* bmpdst, const BITMAP* bmp, int x, int y, uint8_t flip);


/**
 * Limpia la imagen actual a un color especificado
 * @param color el color al que se pinta toda la imagen
 */
void GRAPHICS_ClearBitmap(BITMAP* bmp, uint8_t color);


/**
 * Dibuja una pirámide tridimensional.
 * x, y:	las posiciones x, y de la base de la pirámide
 * sides:   número de lados de la base de la piramide
 * angle:	el angulo de giro de la base de la piramide
 * 			(giro con respecto al eje Z)
 * radius:	el radio de la base (distancia centro-vertice)
 * height:	altura de la pirámide
 */
void GRAPHICS_Draw3DPyramid(BITMAP* bmp, int x, int y,int sides, float angle, float radius, float height, float x_angle);


/**
 * Pinta un rectángulo sin rellenar
 * @param x 
 * @param y coordenadas x,y de la esquina superior izquierda
 * 			del rectangulo
 * @param width ancho del rectángulo
 * @param height altura del rectángulo
 * @param color color del rectángulo
 */
void GRAPHICS_DrawRectangle(BITMAP* bmp, int x1, int y1, int width, int height, uint8_t color);


/**
 * Dibuja una linea recta
 * @param x0
 * @param y0 coordenadas x, y del primer extremo de la recta
 * @param x1
 * @param y1 coordenadas x, y del segundo extremo de la recta
 * @param color color de la recta
 */
void GRAPHICS_DrawLine(BITMAP* bmp, int x0, int y0, int x1, int y1, uint8_t color);


/**
 * Dibuja una circunferencia
 * @param x
 * @param y coordenadas x, y de la circunferencia
 * @param radius radio de la circunferencia
 */
void GRAPHICS_DrawCircle(BITMAP* bmp, float x, float y, float radius, float thickness, uint8_t color);


/**
 * Pinta un pixel en las coordenadas especificadas.
 * Notese que no pasa nada si se especifican coordenadas
 * que están fuera de los márgenes del dibujo.
 * @param x
 * @param y coordenadas en las que se dibuja el pixel
 * @param color color del pixel
 */
static inline void GRAPHICS_PutPixel(BITMAP* bmp, int x, int y, uint8_t color) {

	if (x >= 0 && x < bmp->width && y >= 0 && y < bmp->height)
		bmp->buff[y * bmp->width + x] = color;

}

/**
 * Returns the fps calculated in the last second
 *
 */
uint8_t GetFPS();


/**
 * Swaps the main context buffers using the corresponding function for the 
 * current video driver, if no video driver was selected this function will do
 * nothing. This should be called right after all drawing operations on the 
 * current frame have finished.
 */
void SwapContextBuffers();


/**
 * This function will block until the main context has been drawn by the video 
 * driver. If no video driver is set this function does nothung.
 */
void WaitForVSync();


void DrawBitmap(const BITMAP* bmp, int x, int y);
void ClearBitmap(uint8_t color);
void Draw3DPyramid(int x, int y, int sides, float angle, float radius, float height, float x_angle);
void DrawRectangle(int x1, int y1, int width, int height, uint8_t color);
void DrawFullRectangle(int x, int y, int width, int height, uint8_t color);
//void PutPixel(int x, int y, uint8_t color);
void DrawLine(int x0, int y0, int x1, int y1, uint8_t color);
void DrawCircle(float x, float y, float radius, float thickness, uint8_t color);

extern DRAWING_CONTEXT main_ctx;



//void GRAPHICS_HW_Accel_Init();
//void GRAPHICS_HW_ClearBitmap(BITMAP* bmp, uint8_t color);
//void GRAPHICS_HW_DrawBitmap(BITMAP* bmpdst, BITMAP* bmpsrc, int x, int y);
//void GRAPHICS_DMA2D_ClearBitmap(BITMAP* bmp, uint8_t c);
//bool GRAPHICS_DMA2D_IsAvailable();

#endif