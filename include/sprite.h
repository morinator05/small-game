#ifndef TESTGAME2_SPRITE_H
#define TESTGAME2_SPRITE_H
#include <raylib.h>

#include "animation.h"

#define TILE_SIZE 16

typedef struct
{
    Texture2D texture;
    int frames_per_line;
    Vector2 origin;
    float rotation;
    Animation anim;
} Sprite;

void DrawSprite(Sprite* sprite, Vector2 position);
void UnloadSprite(Sprite* sprite);

#endif //TESTGAME2_SPRITE_H
