//
// Created by mori on 12.07.26.
//

#include "player.h"

void DrawPlayer(Player *player) {
   DrawSprite(&player->sprite);
}

void MovePlayer(Player* player) {
    float distance = player->velocity * 10;
    if (IsKeyDown(KEY_W)) {
        player->sprite.origin.y += distance;
    }
    if (IsKeyDown(KEY_A)) {
        player->sprite.origin.x += distance;
    }
    if (IsKeyDown(KEY_S)) {
        player->sprite.origin.y -= distance;
    }
    if (IsKeyDown(KEY_D)) {
        player->sprite.origin.x -= distance;
    }
}
