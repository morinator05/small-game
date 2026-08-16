//
// Created by mori on 15.08.26.
//

#include <raylib.h>
#include "../include/enemy.h"

#include <raymath.h>

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
        .type = RED_SLIME,
        .sprite = enemy_sprite,
        .position = {0, 0},
        .hp = 100,
        .velocity = 10,
        .angry = false,
        .trigger_radius = 100,
        .hit_radius = 10,
        .hit_rate = 5.0f,
        .hit_cooldown = 0.f,
        .damage_per_hit = 50
    };
}

void UpdateRedSlime(Enemy* enemy, TileMap* map, Player* player)
{
    UpdateAnimation(&enemy->sprite.anim);

    //Update Hit Cooldownd
    enemy->hit_cooldown -= GetFrameTime();
    if (enemy->hit_cooldown < 0) enemy->hit_cooldown = 0;

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

        if (Vector2Distance(player->position, enemy->position) <= enemy->hit_radius && enemy->hit_cooldown <= 0)
        {
            player->hp -= enemy->damage_per_hit;
            enemy->hit_cooldown = enemy->hit_rate;
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

void CreateWiz(Enemy* enemy, Texture2D texture)
{
    Animation enemy_anim = (Animation){
        .first = 0,
        .last = 0,
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
        .type = RED_SLIME,
        .sprite = enemy_sprite,
        .position = {300, 100},
        .hp = 100,
        .velocity = 10,
        .angry = false,
        .trigger_radius = 50,
        .hit_radius = 10,
        .hit_rate = 5.0f,
        .hit_cooldown = 0.f,
        .damage_per_hit = 50
    };
}

void UpdateWiz(Enemy* enemy, TileMap* map, Player* player)
{
}

void UpdateEnemy(Enemy* enemy, TileMap* map, Player* player)
{
    switch (enemy->type)
    {
    case RED_SLIME: UpdateRedSlime(enemy, map, player);
        break;
    case WIZ: UpdateWiz(enemy, map, player);
        break;
    }
}

void DrawEnemy(Enemy* enemy)
{
    DrawSprite(&enemy->sprite, enemy->position);
}
