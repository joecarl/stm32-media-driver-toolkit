#ifndef ENTITIES_H
#define ENTITIES_H

typedef struct {

    //TODO: add sprite assignment
    float speed_v, t_jump, spr_cntr;
    char jumping, spr;

    //public:
    float speed, max_speed, angle;
    float x, y, z;

} Entity;

void EntityInit(Entity* ent);
void EntityMove(Entity* ent);
void EntityProcessControl(Entity* ent,int up, int down, int left, int right);
void EntityJump(Entity* ent);
void EntityDraw(Entity* ent);

#endif