#ifndef FRAMES_H
#define FRAMES_H

#include "libs/graphics.h"

typedef struct {
	BITMAP canvas;
	int x, y;
	int visible;
	char title[30];
} JC_FRAME;

int AddFrame(int x, int y, int width, int height, const char* title);

void DrawFrame(int id);

extern int frame_count;

extern JC_FRAME frames[10];

#endif