//
// Created by mori on 12.07.26.
//

#include "../include/player.h"
#include "../include/tilemap.h"
#include "raymath.h"

void DrawPlayer(Player* player)
{
    DrawSprite(&player->sprite, player->position);
}

void CreatePlayer(Player* player, Texture2D texture)
{
    player->sprite.texture = texture;
    player->sprite.origin = (Vector2){0, 0};
    player->sprite.rotation = 0;
    player->position = (Vector2){0, 0};
    player->velocity = 22;
    player->sprite.frames_per_line = 4;
    player->sprite.anim = (Animation){
        .first = 0,
        .last = 3,
        .current = 0,
        .speed = .1f,
        .time_left = 0
    };
}

void MovePlayer(Player* player, TileMap* tilemap)
{
    //Calculate speed
    float distance = player->velocity * GetFrameTime();
    distance = IsKeyDown(KEY_LEFT_SHIFT) ? 2 * distance : distance;

    Vector2 new_position = player->position;
    Vector2 move_vector = {0, 0};

    //Direction Info for Animation
    static Direction last_direction = DIRECTION_IDLE;
    Direction new_direction = last_direction;
    SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_IDLE, player->sprite.frames_per_line);

    //Get the input
    if (IsKeyDown(KEY_W))
    {
        move_vector.y -= distance;
        SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_UP, player->sprite.frames_per_line);
        new_direction = DIRECTION_UP;
    }
    if (IsKeyDown(KEY_S))
    {
        move_vector.y += distance;
        SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_DOWN, player->sprite.frames_per_line);
        new_direction = DIRECTION_DOWN;
    }
    if (IsKeyDown(KEY_A))
    {
        move_vector.x -= distance;
        SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_LEFT, player->sprite.frames_per_line);
        new_direction = DIRECTION_LEFT;
    }
    if (IsKeyDown(KEY_D))
    {
        move_vector.x += distance;
        SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_RIGHT, player->sprite.frames_per_line);
        new_direction = DIRECTION_RIGHT;
    }

    //Normalize and scale the vector for propper speed when moving diagonally
    move_vector = Vector2Normalize(move_vector);
    move_vector = Vector2Scale(move_vector, distance);

    //Check if the x part of the Movement is allowed
    new_position.x = new_position.x += move_vector.x;
    if (!CheckCollisionWithMap(tilemap, (Rectangle){new_position.x, new_position.y,TILE_SIZE,TILE_SIZE}))
    {
        player->position.x += move_vector.x;
    }
    new_position.x = player->position.x;

    //Check if the y part of the Movement is allowed
    new_position.y = new_position.y += move_vector.y;
    if (!CheckCollisionWithMap(tilemap, (Rectangle){new_position.x, new_position.y,TILE_SIZE,TILE_SIZE}))
    {
        player->position.y += move_vector.y;
    }

    if (new_direction != last_direction)
    {
        last_direction = new_direction;
        Reset(&player->sprite.anim);
    }
}
