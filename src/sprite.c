//
// Created by mori on 12.07.26.
//

#include <raylib.h>
#include "../include/sprite.h"

void DrawSprite(Sprite *sprite) {
    DrawTexturePro(sprite->texture, sprite->source, sprite->destination, sprite->origin, sprite->rotation, WHITE);
}
void UnloadSprite(Sprite *sprite) {

}