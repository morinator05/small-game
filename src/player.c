//
// Created by mori on 12.07.26.
//

#include "../include/player.h"

#include "../include/tilemap.h"

void DrawPlayer(Player *player){
    DrawSprite(&player->sprite, player->position);
}

void CreatePlayer(Player *player, Texture2D texture) {
    player->sprite.texture = texture;
    player->sprite.source = (Rectangle){0, 0, 16, 16};
    player->sprite.origin = (Vector2){0, 0};
    player->sprite.rotation = 0;
    player->position = (Vector2){0,0};
    player->velocity = 42;
}

void MovePlayer(Player *player, TileMap *tilemap)
{
    float distance = player->velocity * GetFrameTime();

    Vector2 new_position = player->position;

    if (IsKeyDown(KEY_W))
    {
        new_position.y -= distance;
        SetDirection(player, DIRECTION_UP);
    }

    if (IsKeyDown(KEY_S))
    {
        new_position.y += distance;
        SetDirection(player, DIRECTION_DOWN);
    }

    if (IsKeyDown(KEY_A))
    {
        new_position.x -= distance;
        SetDirection(player, DIRECTION_LEFT);
    }

    if (IsKeyDown(KEY_D))
    {
        new_position.x += distance;
        SetDirection(player, DIRECTION_RIGHT);
    }

    if (!CheckCollisionWithMap(tilemap, new_position))
    {
        player->position = new_position;
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
