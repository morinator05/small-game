//
// Created by mori on 12.07.26.
//

#include "player.h"

void DrawPlayer(Player *player) {
    DrawSprite(&player->sprite);
}

void CreatePlayer(Player *player, Texture2D texture) {
    player->sprite.texture = texture;
    player->sprite.source = (Rectangle){0, 0, 16, 16};
    player->sprite.destination = (Rectangle){16, 16, 16, 16};
    player->sprite.origin = (Vector2){0, 0};
    player->sprite.rotation = 0;
    player->velocity = 200;
}

void MovePlayer(Player *player) {
    float distance = player->velocity * GetFrameTime();
    if (IsKeyDown(KEY_W)) {
        player->sprite.origin.y += distance;
        SetDirection(player, DIRECTION_UP);
    }
    if (IsKeyDown(KEY_A)) {
        player->sprite.origin.x += distance;
        SetDirection(player, DIRECTION_LEFT);
    }
    if (IsKeyDown(KEY_S)) {
        player->sprite.origin.y -= distance;
        SetDirection(player, DIRECTION_DOWN);
    }
    if (IsKeyDown(KEY_D)) {
        player->sprite.origin.x -= distance;
        SetDirection(player, DIRECTION_RIGHT);
    }
}

void SetDirection(Player *player, Direction direction) {
    switch (direction)
    {
        case DIRECTION_DOWN:
            player->sprite.source.x = 0;
            break;

        case DIRECTION_UP:
            player->sprite.source.x = 16;
            break;

        case DIRECTION_LEFT:
            player->sprite.source.x = 32;
            break;

        case DIRECTION_RIGHT:
            player->sprite.source.x = 48;
            break;


    }
}
