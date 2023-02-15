/**
 * ARCHIVO: "objects.c"
 * NOMBRE:  Objetos
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo contiene funciones para manejar las estructuras definidas
 *		en "objects.h". Está pensado para el control movimiento y físicas de 
 *		personajes de videojuegos.
 *
 */

#include <math.h>
#include "stm32f4xx.h"
#include "libs/graphics.h"
#include "libs/objects.h"


void JC_CHAR_Init(JC_CHAR* character)
{
	character->max_speed = 3;
	character->x = character->y = 270;
	//z es la coordenada vertical. lo hacemos así por que está pensado para juegos 2D
	character->t_jump = character->z = character->speed = character->speed_v = character->angle = 0; 
	character->spr = 0;
	character->spr_cntr = 0;

}


void Move(JC_CHAR* character)
{
	character->x += character->speed * cos(character->angle);
	character->y -= character->speed * sin(character->angle);
	if (character->jumping) {
		character->z += character->speed_v - 9.8 / 2 * character->t_jump;
		character->t_jump += 0.05;
		if (character->z < 0) {
			character->z = 0;
			character->jumping = false;
			character->t_jump = 0;
			character->spr = 0;
		}
	}
	//calculo del sprite:
	else//if (!character->jumping)
	{
		if (character->speed != 0)
		{
			character->spr_cntr += character->speed / 5;
			if (character->spr_cntr > 4) {
				character->spr++;
				character->spr_cntr = 0;
			}
			if (character->spr > 2)
				character->spr = 0;
		}
		else {
			character->spr = 0;
		}
	}
}


void ProcessControl(JC_CHAR* character, int up, int down, int left, int right)
{
	float vert = up - down;
	float horiz = right - left;

	float force_angle = atan2(vert, horiz);
	float force;
	if (vert || horiz)
		force = 0.19;
	else
		force = 0;

	if (character->speed > 0.07)
		character->speed -= 0.07;//Rozamiento
	else
		character->speed = 0;

	float speed_x = character->speed * cos(character->angle) + (force / 1) * cos(force_angle);
	float speed_y = character->speed * sin(character->angle) + (force / 1) * sin(force_angle);

	character->speed = sqrt(speed_x * speed_x + speed_y * speed_y);
	if (character->speed > character->max_speed) character->speed = character->max_speed;

	character->angle = atan2(speed_y, speed_x);

}


void Jump(JC_CHAR* character)
{
	//t_jump = 0;
	character->speed_v = 5.5;
	character->jumping = true;
	character->spr = 3;//dibujo saltando
}


void Draw(JC_CHAR* character)
{
	//DrawMario(character->x,character->y-character->z,character->spr);
	//al_draw_filled_ellipse(x, y-4, 30, 8, GREY);//shadow
	//al_draw_filled_circle(x, y-z, 17, WHITE);
	//JC_al_draw_bitmap(spr, x, y-z,0.7,0, 0,false);
}
