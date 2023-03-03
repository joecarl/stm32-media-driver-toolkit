#ifndef MDT_VGA_DRIVER_H
#define MDT_VGA_DRIVER_H

#include <stm32f4xx.h>

#define MDT_VGA_640x400 20
#define MDT_VGA_640x480 21


/**
 * A structure for handling the active rendering state
 */
typedef struct {

	/**
	 * Number of screen refreshes done since the driver was initialized
	 */
	uint32_t screen_refresh_count;

	/**
	 * Number of rendered lines (blanking & video) in the current frame
	 */
	uint16_t screen_lines_done;

	/**
	 * Number of rendered video lines in the current frame
	 */
	uint16_t video_lines_done;

	/**
	 * This variable is set to 1 when the VSYNC interrupt is emmited, and it is
	 * set to 0 in the WaitForVSYNC function, which will keep waiting until it 
	 * is set to 1 again  
	 */
	uint8_t v_sync_done;

} MDT_VGA_RENDER_STATE;


/**
 * A structure for storing the vga active mode configuration
 * Have a look at this site for reference: 
 * http://javiervalcarce.eu/html/vga-signal-format-timming-specs-en.html
 */
typedef struct {

	/**
	 * The refresh rate in Hz
	 */
	uint8_t refresh_rate;

	/**
	 * Number of screen lines
	 */
	uint16_t screen_lines;

	/**
	 * Number of video lines. It should match the Y resolution unless
	 * the mode is downscaled.
	 */
	uint16_t video_lines;

} MDT_VGA_MODE;



typedef struct {

	/**
	 * Modo de señal VGA. Puede tomar los valores siguientes:
	 * 		MDT_VGA_640x400
	 * 		MDT_VGA_640x480
	 *
	 */
	uint8_t mode;

	uint8_t** bufferPointer;

	uint16_t bufferColumns;

	uint16_t bufferRows;

} MDT_VGA_InitTypedef;


/**
 * Inicializa la señal VGA
 * @param res especifica la resolucion puede tomar los valores siguientes:
 *
 * 		MDT_VGA_640x400
 * 		MDT_VGA_640x480
 *
 */
void MDT_VGA_Init(MDT_VGA_InitTypedef* config);


/**
 * Deinitializes the VGA driver and all the related hardware
 */
void MDT_VGA_DeInit();


/**
 * Detiene el código que se está ejecutando hasta que se envíe
 * la siguiente señal de sincronismo vertical
 */
void MDT_VGA_WaitForVSync();


#endif
