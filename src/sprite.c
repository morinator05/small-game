//
// Created by mori on 12.07.26.
//

#include <raylib.h>
#include "../include/sprite.h"

void DrawSprite(Sprite* sprite, const Vector2 position)
{
    UpdateAnimation(&sprite->anim);

    Rectangle destination = {
        position.x,
        position.y,
        TILE_SIZE,
        TILE_SIZE
    };

    DrawTexturePro(
        sprite->texture,
        GetCurrentFrame(&sprite->anim, sprite->frames_per_line),
        destination,
        sprite->origin,
        sprite->rotation,
        WHITE
    );
}

void UnloadSprite(Sprite* sprite)
{
}
