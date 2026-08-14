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
    player->sprite.origin = (Vector2){0, 0};
    player->sprite.rotation = 0;
    player->position = (Vector2){0, 0};
    player->velocity = 22;
    player->sprite.frames_per_line = 4;
    player->sprite.anim = (Animation) {
        .first = 0,
        .last = 3,
        .current = 0,
        .speed = .1f,
        .time_left = 0
    };
}

void MovePlayer(Player *player, TileMap *tilemap) {
    float distance = player->velocity * GetFrameTime();
    distance = IsKeyDown(KEY_LEFT_SHIFT) ? 2 * distance : distance;
    Vector2 new_position = player->position;
    static Direction last_direction = DIRECTION_IDLE;
    Direction new_direction = last_direction;

    //TODO: fix vertical movement speed
    //TODO: split x and y movement

    SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_IDLE, player->sprite.frames_per_line);
    new_direction = DIRECTION_IDLE;

    if (IsKeyDown(KEY_W)) {
        new_position.y -= distance;
        SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_UP, player->sprite.frames_per_line);
        new_direction = DIRECTION_UP;
    }
    if (IsKeyDown(KEY_S)) {
        new_position.y += distance;
        SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_DOWN, player->sprite.frames_per_line);
        new_direction = DIRECTION_DOWN;
    }
    if (IsKeyDown(KEY_A)) {
        new_position.x -= distance;
        SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_LEFT, player->sprite.frames_per_line);
        new_direction = DIRECTION_LEFT;
    }
    if (IsKeyDown(KEY_D)) {
        new_position.x += distance;
        SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_RIGHT, player->sprite.frames_per_line);
        new_direction = DIRECTION_RIGHT;
    }

    Rectangle playerRect = {
        new_position.x,
        new_position.y,
        TILE_SIZE,
        TILE_SIZE
    };

    if (!CheckCollisionWithMap(tilemap, playerRect)) {
        player->position = new_position;
    }

    if (new_direction != last_direction)
    {
        last_direction = new_direction;
        Reset(&player->sprite.anim);
    }

}