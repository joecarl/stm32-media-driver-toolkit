#ifndef MDT_ENTITY_H
#define MDT_ENTITY_H

#include "mdt/graphics.h"

typedef struct {

	const MDT_BITMAP* sprites[16];
	//uint8_t jumping_spr_index;
	uint8_t spr;
	float spr_cntr;

	float force;

	float force_angle;

    uint8_t jumping;
    float speed; 
	float angle;
	float max_speed; 
    float speed_v;
    float x, y, z;

} MDT_ENTITY;

void MDT_ENTITY_Init(MDT_ENTITY* ent);
void MDT_ENTITY_ProcessPhysics(MDT_ENTITY* ent, float time_inc);
void MDT_ENTITY_ProcessControl(MDT_ENTITY* ent, int up, int down, int left, int right);
void MDT_ENTITY_Jump(MDT_ENTITY* ent);
void MDT_ENTITY_Draw(MDT_ENTITY* ent);

#endif