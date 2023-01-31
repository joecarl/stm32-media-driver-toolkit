#ifndef OBJECTS_H
#define OBJECTS_H

#define true 1
#define false 0

typedef struct {

    //ALLEGRO_BITMAP* spr;
    float speed_v, t_jump, spr_cntr;
    char jumping, spr;

    //public:
    float speed, max_speed, angle;
    float x, y, z;

} JC_CHAR;

void JC_CHAR_Init(JC_CHAR* character);
void Move(JC_CHAR* character);
void ProcessControl(JC_CHAR* character,int up, int down, int left, int right);
void Jump(JC_CHAR* character);
void Draw(JC_CHAR* character);

#endif