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

#include <stm32f4xx.h>
#include "libs/graphics.h"
#include "libs/entity.h"
#include <math.h>
#include <stdbool.h>


void MDT_ENTITY_Init(MDT_ENTITY* ent) {
	
	ent->max_speed = 3;
	ent->x = ent->y = 270;
	//z es la coordenada vertical. lo hacemos así por que está pensado para juegos 2D
	ent->t_jump = ent->z = ent->speed = ent->speed_v = ent->angle = 0; 
	ent->spr = 0;
	ent->spr_cntr = 0;

}


void MDT_ENTITY_Move(MDT_ENTITY* ent) {

	ent->x += ent->speed * cos(ent->angle);
	ent->y -= ent->speed * sin(ent->angle);
	if (ent->jumping) {
		ent->z += ent->speed_v - 9.8 / 2 * ent->t_jump;
		ent->t_jump += 0.05;
		if (ent->z < 0) {
			ent->z = 0;
			ent->jumping = false;
			ent->t_jump = 0;
			ent->spr = 0;
		}
	}
	//calculo del sprite:
	else//if (!ent->jumping)
	{
		if (ent->speed != 0)
		{
			ent->spr_cntr += ent->speed / 5;
			if (ent->spr_cntr > 4) {
				ent->spr++;
				ent->spr_cntr = 0;
			}
			if (ent->spr > 2)
				ent->spr = 0;
		}
		else {
			ent->spr = 0;
		}
	}
}


void MDT_ENTITY_ProcessControl(MDT_ENTITY* ent, int up, int down, int left, int right) {

	float vert = up - down;
	float horiz = right - left;

	float force_angle = atan2(vert, horiz);
	float force;
	if (vert || horiz)
		force = 0.19;
	else
		force = 0;

	if (ent->speed > 0.07)
		ent->speed -= 0.07;//Rozamiento
	else
		ent->speed = 0;

	float speed_x = ent->speed * cos(ent->angle) + (force / 1) * cos(force_angle);
	float speed_y = ent->speed * sin(ent->angle) + (force / 1) * sin(force_angle);

	ent->speed = sqrt(speed_x * speed_x + speed_y * speed_y);
	if (ent->speed > ent->max_speed) ent->speed = ent->max_speed;

	ent->angle = atan2(speed_y, speed_x);

}


void MDT_ENTITY_Jump(MDT_ENTITY* ent) {

	//t_jump = 0;
	ent->speed_v = 5.5;
	ent->jumping = true;
	ent->spr = 3;//dibujo saltando

}


void MDT_ENTITY_Draw(MDT_ENTITY* ent) {

	//draw_filled_ellipse(x, y - 4, 30, 8, GREY);//shadow
	//draw_filled_circle(x, y - z, 17, WHITE);
	//draw_bitmap(ent->x, ent->y - ent->z, ent->spr);

}
