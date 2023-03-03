#ifndef MDT_SPRITES_H
#define MDT_SPRITES_H

#include "mdt/graphics.h"

void DrawMario(MDT_BITMAP* bmp, int x, int y, int spr);
void DrawSuelo(MDT_BITMAP* bmp, int x, int y);
void DrawTuberia(MDT_BITMAP* bmp, int x, int y, int height);
void DrawLogo(int x, int y);
void DrawCreature(int x, int y);

#endif