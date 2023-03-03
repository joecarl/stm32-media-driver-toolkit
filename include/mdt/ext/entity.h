#ifndef MDT_ENTITY_H
#define MDT_ENTITY_H

typedef struct {

    //TODO: add sprite assignment
    float speed_v, t_jump, spr_cntr;
    uint8_t jumping, spr;

	float force;

	float force_angle;

    float speed, max_speed, angle;
    float x, y, z;

} MDT_ENTITY;

void MDT_ENTITY_Init(MDT_ENTITY* ent);
void MDT_ENTITY_ProcessPhysics(MDT_ENTITY* ent, float time_inc);
void MDT_ENTITY_ProcessControl(MDT_ENTITY* ent, int up, int down, int left, int right);
void MDT_ENTITY_Jump(MDT_ENTITY* ent);
void MDT_ENTITY_Draw(MDT_ENTITY* ent);

#endif