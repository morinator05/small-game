#include "../include/animation.h"

void UpdateAnimation(Animation* anim)
{
    const float delta = GetFrameTime();
    anim->time_left -= delta;

    if (anim->time_left <= 0.0f)
    {
        anim->current += 1;
        anim->time_left = anim->speed;

        if (anim->current > anim->last)
        {
            anim->current = anim->first;
        }
    }
}

Rectangle GetCurrentFrame(const Animation* animation, const int frames_per_row)
{
    const float x = (animation->current % frames_per_row) * 16;
    const float y = (animation->current / frames_per_row) * 16;

    return (Rectangle){
        .x = x,
        .y = y,
        .width = 16,
        .height = 16
    };
}


void SetRow(Animation* anim, const int row, const int frames_per_row)
{
    anim->first = row * frames_per_row;
    anim->last = anim->first + frames_per_row - 1;

    if (anim->current < anim->first || anim->current > anim->last)
    {
        anim->current = anim->first;
    }
}

void Reset(Animation* anim)
{
    anim->current = anim->first;
}
