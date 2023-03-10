#ifndef MDT_FRAME_H
#define MDT_FRAME_H

#include "mdt/graphics.h"

typedef struct {
	MDT_BITMAP canvas;
	int x, y;
	int visible;
	char title[30];
} MDT_FRAME;

int AddFrame(int x, int y, int width, int height, const char* title);

void DrawFrame(int id);

extern int frame_count;

extern MDT_FRAME frames[10];

#endif