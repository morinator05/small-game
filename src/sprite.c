//
// Created by mori on 12.07.26.
//

#include <raylib.h>
#include "../include/sprite.h"

void DrawSprite(Sprite *sprite, Vector2 position) {
    Rectangle destination = {
        position.x,
        position.y,
        TILE_SIZE,
       TILE_SIZE
    };

    DrawTexturePro(
        sprite->texture,
        sprite->source,
        destination,
        sprite->origin,
        sprite->rotation,
        WHITE
    );
}
void UnloadSprite(Sprite *sprite) {

}