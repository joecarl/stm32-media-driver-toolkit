#ifndef MDT_SPRITES_H
#define MDT_SPRITES_H

#include "mdt/graphics.h"

void DrawMario(BITMAP* bmp, int x, int y, int spr);
void DrawSuelo(BITMAP* bmp, int x, int y);
void DrawTuberia(BITMAP* bmp, int x, int y, int height);
void DrawLogo(int x, int y);
void DrawCreature(int x, int y);

#endif