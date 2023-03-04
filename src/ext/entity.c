/**
 * ARCHIVO: "entity.c"
 * NOMBRE:  Entidades
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo contiene funciones para manejar las estructuras definidas
 *		en "entity.h". Está pensado para el control, movimiento y físicas de 
 *		personajes y objetos en videojuegos.
 *
 */

#include "mdt/ext/entity.h"
#include <math.h>
#include <stdbool.h>

#define PPM 20


void MDT_ENTITY_Init(MDT_ENTITY* ent) {
	
	ent->max_speed = 6;
	ent->x = ent->y = 270;
	//z es la coordenada vertical. lo hacemos así por que está pensado para juegos 2D
	ent->z = ent->speed = ent->speed_v = ent->angle = 0; 
	ent->spr = 0;
	ent->spr_cntr = 0;

}


void MDT_ENTITY_ProcessPhysics(MDT_ENTITY* ent, float time_inc) {

	ent->x += ent->speed * cos(ent->angle) * time_inc * PPM;
	ent->y -= ent->speed * sin(ent->angle) * time_inc * PPM;

	if (ent->jumping) {

		ent->speed_v -= 9.8 * time_inc * 2.9;
		ent->z += ent->speed_v * time_inc * PPM;
		
		if (ent->z < 0) {
			ent->z = 0;
			ent->jumping = false;
		}

	}
	
	if (ent->speed > 0)
		ent->speed -= 0.27 * time_inc * PPM;//Rozamiento

	if (ent->speed < 0)
		ent->speed = 0;

	float speed_x = ent->speed * cos(ent->angle) + (ent->force / 1) * cos(ent->force_angle) * time_inc * PPM;
	float speed_y = ent->speed * sin(ent->angle) + (ent->force / 1) * sin(ent->force_angle) * time_inc * PPM;

	ent->speed = sqrt(speed_x * speed_x + speed_y * speed_y);
	if (ent->speed > ent->max_speed) ent->speed = ent->max_speed;

	ent->angle = atan2(speed_y, speed_x);

}


void MDT_ENTITY_ProcessControl(MDT_ENTITY* ent, int up, int down, int left, int right) {

	float vert = up - down;
	float horiz = right - left;

	ent->force_angle = atan2(vert, horiz);
	
	if (vert || horiz)
		ent->force = 0.68;
	else
		ent->force = 0;

}


void MDT_ENTITY_Jump(MDT_ENTITY* ent) {

	if (ent->jumping) {
		return;
	}
	ent->speed_v = 12.8;
	ent->jumping = true;
	ent->spr = 3;//dibujo saltando

}


void MDT_ENTITY_Draw(MDT_ENTITY* ent) {

	//draw_filled_ellipse(x, y - 4, 30, 8, GREY);//shadow
	//draw_filled_circle(x, y - z, 17, WHITE);//ball

	const MDT_BITMAP* spr;

	if (ent->jumping) {

		spr = ent->sprites[3];

	} else {

		if (ent->speed != 0) {
			ent->spr_cntr += ent->speed / 5;
			if (ent->spr_cntr > 4) {
				ent->spr++;
				ent->spr_cntr = 0;
			}
			if (ent->spr > 2)
				ent->spr = 0;
		} else {
			ent->spr = 0;
		}

		spr = ent->sprites[ent->spr];

	}
	
	MDT_DrawBitmap(spr, (int) ent->x, (int) (ent->y - ent->z));

}
