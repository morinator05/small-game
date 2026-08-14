//
// Created by mori on 14.08.26.
//

#ifndef TESTGAME2_ANIMATION_H
#define TESTGAME2_ANIMATION_H
#include <raylib.h>

typedef struct
{
   int first;
   int last;
   int current;

   float speed;
   float time_left;
}Animation;

void SetRow(Animation * anim, int row, int frames_per_row);

void Reset(Animation *anim);

void UpdateAnimation(Animation *animation);

Rectangle GetCurrentFrame(Animation *animation, int frames_per_row);

#endif //TESTGAME2_ANIMATION_H
