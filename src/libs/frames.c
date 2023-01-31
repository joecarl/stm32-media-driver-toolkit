#include "stm32f4xx.h"
#include "libs/graphics.h"
#include "libs/text.h"
#include "libs/frames.h"
#include <string.h>


int frame_count = 0;
JC_FRAME frames[10];

int AddFrame(int x, int y, int width, int height, const char* title)
{
	BITMAP_Init(&(frames[frame_count].canvas), height, width);
	frames[frame_count].x = x;
	frames[frame_count].y = y;
	frames[frame_count].visible = 1;
	strcpy(frames[frame_count].title, title);
	frame_count++;
	return frame_count-1;
}

void DrawFrame(int id)
{
	
	if (!frames[id].visible) {
		return;
	}

	DrawBitmap(&(frames[id].canvas), frames[id].x, frames[id].y);
	//BITMAP_HW_DrawBitmap(&background,&(frames[id].canvas),frames[id].x,frames[id].y);
	DrawRectangle(frames[id].x - 1, frames[id].y - 12, frames[id].canvas.width + 1, frames[id].canvas.height + 12, 0xFF);
	DrawFullRectangle(frames[id].x, frames[id].y - 11, frames[id].canvas.width, 11, 0x1F);

	DrawText(frames[id].title, frames[id].x + 2, frames[id].y - 9, 0xFF);
	DrawText("x", frames[id].x + frames[id].canvas.width - 7, frames[id].y - 9, 0xFF);
	DrawLine(frames[id].x, frames[id].y, frames[id].x + frames[id].canvas.width, frames[id].y, 0xFF);
	
}
