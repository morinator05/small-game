//
// Created by mori on 14.08.26.
//

#include "../include/animation.h"

void UpdateAnimation(Animation *animation)
{
    float delta = GetFrameTime();
    animation->time_left -= delta;

    if (animation->time_left <= 0.0f)
    {
        animation->current += 1;
        animation->time_left = animation->speed;

        if (animation->current > animation->last)
        {
            animation->current = animation->first;
        }
    }
}

Rectangle GetCurrentFrame(Animation *animation, int frames_per_row)
{
    float x = (animation->current % frames_per_row) * 16;
    float y = (animation->current / frames_per_row) * 16;

    return (Rectangle) {
        .x = x,
        .y = y,
        .width = 16,
        .height = 16
    };
}


void SetRow(Animation * anim, int row, int frames_per_row)
{
    anim->first = row * frames_per_row;
    anim->last = anim->first + frames_per_row - 1;
}

void Reset(Animation * anim)
{
    anim-> current = anim->first;
}