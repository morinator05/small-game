//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_SPRITE_H
#define TESTGAME2_SPRITE_H
#include <raylib.h>

#define TILE_SIZE 16

typedef struct {
    Texture2D texture;
    Rectangle source;
    Vector2 origin;
    float rotation;
    float velocity;
} Sprite;

void DrawSprite(Sprite *sprite, Vector2 position);
void UnloadSprite(Sprite *sprite);

#endif //TESTGAME2_SPRITE_H