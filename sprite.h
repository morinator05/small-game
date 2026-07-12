//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_SPRITE_H
#define TESTGAME2_SPRITE_H
#include <raylib.h>

typedef struct {
    Texture2D texture;
    Rectangle source;
    Rectangle destination;
    Vector2 origin;
    float rotation;
    float velocity;
} Sprite;

void DrawSprite(Sprite *sprite);
void UnloadSprite(Sprite *sprite);

#endif //TESTGAME2_SPRITE_H