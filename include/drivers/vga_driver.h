#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H


#define VGA_320x200 10
#define VGA_320x240 11
#define VGA_640x400 20
#define VGA_640x480 21


/**
 * A structure for handling the active rendering state
 */
typedef struct {

	/**
	 * The number of frames rendered in the last second (this is a valid 
	 * value for retrieving the fps)
	 */
	uint8_t fps;

	/**
	 * Number of frames rendered in the current second. this values is used to
	 * calculate the final fps value. It is incremented after each rendered 
	 * frame and reseted to 0 after each second.
	 */
	uint8_t fps_counter;

	/**
	 * Number of screen refreshes done in the current second. it is used to 
	 * get
	 */
	uint8_t real_fps_counter;

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

} VGA_RENDER_STATE;


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

} VGA_MODE;



typedef struct {

	/**
	 * Modo de señal VGA. Puede tomar los valores siguientes:
	 * 		VGA_320x200
	 * 		VGA_320x240
	 * 		VGA_640x400
	 * 		VGA_640x480
	 *
	 */
	uint8_t mode;

	uint8_t** bufferPointer;

	uint16_t bufferColumns;

	uint16_t bufferRows;

} VGA_InitTypedef;


void VGA_Init(VGA_InitTypedef* config);

void VGA_WaitForVSync();

uint8_t VGA_GetFPS();


#endif
