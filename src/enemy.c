//
// Created by mori on 15.08.26.
//

#include <raylib.h>
#include "../include/enemy.h"

#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

void CreateRedSlime(Enemy* enemy, Texture2D texture)
{
    Animation enemy_anim = (Animation){
        .first = 0,
        .last = 3,
        .current = 0,
        .speed = 0.4f,
        .time_left = 0
    };

    Sprite enemy_sprite = (Sprite){
        .texture = texture,
        .frames_per_line = 4,
        .origin = {0, 0},
        .rotation = 0.0f,
        .anim = enemy_anim
    };

    *enemy = (Enemy){
        .sprite = enemy_sprite,
        .position = {100, 100},
        .hp = 100,
        .velocity = 10,
        .angry = false,
        .trigger_radius = 50,
        .hit_radius = 10
    };
}

void UpdateRedSlime(Enemy* enemy, TileMap* map, const Player* player)
{
    UpdateAnimation(&enemy->sprite.anim);

    const float distance_to_player = Vector2Distance(enemy->position, player->position);
    if (distance_to_player <= enemy->trigger_radius)
    {
        enemy->angry = true;
        const float distance = enemy->velocity * GetFrameTime();
        const Vector2 new_position = Vector2MoveTowards(enemy->position, player->position, distance);

        const bool block_x = CheckCollisionWithMap(map, (Rectangle){
                                                       .x = new_position.x, .y = enemy->position.y, .width = 16,
                                                       .height = 16
                                                   });


        const bool block_y = CheckCollisionWithMap(map, (Rectangle){
                                                       .x = enemy->position.x, .y = new_position.y, .width = 16,
                                                       .height = 16
                                                   });

        if (!block_x)
        {
            enemy->position.x = new_position.x;
        }
        if (!block_y)
        {
            enemy->position.y = new_position.y;
        }

        if (Vector2Distance(player->position, enemy->position) <= enemy->hit_radius)
        {
            printf("Hit");
        }
    }
    else
    {
        enemy->angry = false;
    }

    if (!enemy->angry)
    {
        SetRow(&enemy->sprite.anim, TEXTURE_ENEMY_ROW_IDLE, enemy->sprite.frames_per_line);
    }
    else
    {
        SetRow(&enemy->sprite.anim, TEXTURE_ENEMY_ROW_ANGRY, enemy->sprite.frames_per_line);
    }
}

void DrawEnemy(Enemy* enemy)
{
    DrawSprite(&enemy->sprite, enemy->position);
}
