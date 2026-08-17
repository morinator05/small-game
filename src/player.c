//
// Created by mori on 12.07.26.
//

#include "../include/player.h"

#include "../include/tilemap.h"
#include "raymath.h"
#include "../include/assets.h"

void DrawPlayer(Player* player)
{
    DrawSprite(&player->sprite, player->position);
}

void CreatePlayer(Player* player, const Texture2D texture)
{
    player->sprite.texture = texture;
    player->sprite.origin = (Vector2){0, 0};
    player->sprite.rotation = 0;
    player->position = (Vector2){0, 0};
    player->velocity = 22;
    player->sprite.frames_per_line = FRAMES_PER_LINE;
    player->sprite.anim = (Animation){
        .first = 0,
        .last = 3,
        .current = 0,
        .speed = .2f,
        .time_left = 0
    };
    player->hp = 1000.0f;
}

void MovePlayer(Player* player, const TileMap* tilemap)
{
    //Calculate speed
    float distance = player->velocity * GetFrameTime();
    distance = IsKeyDown(KEY_LEFT_SHIFT) ? 2 * distance : distance;

    Vector2 new_position = player->position;
    Vector2 move_vector = {0, 0};
    Direction current_direction = IDLE;

    //Get the input
    if (IsKeyDown(KEY_W))
    {
        move_vector.y -= distance;
        current_direction = UP;
    }
    if (IsKeyDown(KEY_S))
    {
        move_vector.y += distance;
        current_direction = DOWN;
    }
    if (IsKeyDown(KEY_A))
    {
        move_vector.x -= distance;
        current_direction = LEFT;
    }
    if (IsKeyDown(KEY_D))
    {
        move_vector.x += distance;
        current_direction = RIGHT;
    }

    //Normalize and scale the vector for propper speed when moving diagonally
    if (current_direction != IDLE)
    {
        move_vector = Vector2Normalize(move_vector);
        move_vector = Vector2Scale(move_vector, distance);
    }
    float length = Vector2Length(move_vector);

    if (length > 0.0f)
    {
        //Check if the x part of the Movement is allowed
        new_position.x += move_vector.x;
        if (!CheckCollisionWithMap(tilemap, (Rectangle){new_position.x, new_position.y,TILE_SIZE,TILE_SIZE}))
        {
            player->position.x += move_vector.x;
        }
        new_position.x = player->position.x;

        //Check if the y part of the Movement is allowed
        new_position.y += move_vector.y;
        if (!CheckCollisionWithMap(tilemap, (Rectangle){new_position.x, new_position.y,TILE_SIZE,TILE_SIZE}))
        {
            player->position.y += move_vector.y;
        }
    }
    else
    {
        current_direction = IDLE;
    }

    switch (current_direction)
    {
    case UP: SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_UP, player->sprite.frames_per_line);
        break;
    case DOWN: SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_DOWN, player->sprite.frames_per_line);
        break;
    case LEFT: SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_LEFT, player->sprite.frames_per_line);
        break;
    case RIGHT: SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_RIGHT, player->sprite.frames_per_line);
        break;
    case IDLE: SetRow(&player->sprite.anim, TEXTURE_PLAYER_ROW_IDLE, player->sprite.frames_per_line);
        break;
    }

    if (player->current_direction != current_direction)
    {
        Reset(&player->sprite.anim);
        player->current_direction = current_direction;
    }
}
