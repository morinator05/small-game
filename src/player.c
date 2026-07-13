//
// Created by mori on 12.07.26.
//

#include "../include/player.h"

#include "../include/tilemap.h"

void DrawPlayer(Player *player) {
    DrawSprite(&player->sprite, player->position);
}

void CreatePlayer(Player *player, Texture2D texture) {
    player->sprite.texture = texture;
    player->sprite.source = (Rectangle){0, 0, 16, 16};
    player->sprite.origin = (Vector2){0, 0};
    player->sprite.rotation = 0;
    player->position = (Vector2){0, 0};
    player->velocity = 22;
    player->animationTimer = .0f;
    player->currentFrame = 0;
}

void MovePlayer(Player *player, TileMap *tilemap) {
    bool moving = false;
    float distance = player->velocity * GetFrameTime();
    distance = IsKeyDown(KEY_LEFT_SHIFT) ? 2 * distance : distance;
    Vector2 new_position = player->position;

    if (IsKeyDown(KEY_W)) {
        new_position.y -= distance;
        moving = true;
        SetDirection(player, DIRECTION_UP);
    }
    if (IsKeyDown(KEY_S)) {
        new_position.y += distance;
        moving = true;
        SetDirection(player, DIRECTION_DOWN);
    }
    if (IsKeyDown(KEY_A)) {
        new_position.x -= distance;
        moving = true;
        SetDirection(player, DIRECTION_LEFT);
    }
    if (IsKeyDown(KEY_D)) {
        new_position.x += distance;
        moving = true;
        SetDirection(player, DIRECTION_RIGHT);
    }

    if (moving) {
        player->animationTimer += GetFrameTime();
        if (player->animationTimer >= 0.15f) {
            player->currentFrame++;
            player->currentFrame %= 4;

            player->animationTimer = 0.0f;
        }
    } else {
        player->currentFrame = 0; // Idle
    }

    player->sprite.source.x = player->currentFrame * TILE_SIZE;

    Rectangle playerRect = {
        new_position.x,
        new_position.y,
        TILE_SIZE,
        TILE_SIZE
    };

    if (!CheckCollisionWithMap(tilemap, playerRect)) {
        player->position = new_position;
    }
}

void SetDirection(Player *player, Direction direction) {
    switch (direction) {
        case DIRECTION_DOWN:
            player->sprite.source.y = 0;
            break;

        case DIRECTION_UP:
            player->sprite.source.y = 16;
            break;

        case DIRECTION_LEFT:
            player->sprite.source.y = 32;
            break;

        case DIRECTION_RIGHT:
            player->sprite.source.y = 48;
            break;
    }
}
