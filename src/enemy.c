#include <raylib.h>
#include "../include/enemy.h"
#include <raymath.h>
#include "../include/player.h"

#include "../include/assets.h"

void CreateRedSlime(Enemy* enemy)
{
    const auto enemy_anim = (Animation){
        .first = ANIM_FRAME_FIRST,
        .last = ANIM_FRAME_LAST,
        .speed = ANIM_SPEED,
    };

    const auto enemy_sprite = (Sprite){
        .texture = assets.spritesheet_red_slime,
        .frames_per_line = ANIM_FRAMES_PER_LINE,
        .anim = enemy_anim
    };

    *enemy = (Enemy){
        .type = RED_SLIME,
        .sprite = enemy_sprite,
        .alive = true,
        .hp = ENEMY_RED_SLIME_HP,
        .max_hp = ENEMY_RED_SLIME_HP,
        .velocity = ENEMY_RED_SLIME_VELOCITY,
        .angry = false,
        .trigger_radius = ENEMY_RED_SLIME_TRIGGER_DISTANCE,
        .hit_radius = ENEMY_HIT_RADIUS,
        .hit_rate = ENEMY_RED_SLIME_HIT_RATE,
        .damage_per_hit = ENEMY_RED_SLIME_DAMAGE
    };
}

void UpdateRedSlime(Enemy* enemy, TileMap* map, Player* player)
{
    //Update Hit Cooldown
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
            PlaySound(assets.sfx_hit);
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
        SetRow(&enemy->sprite.anim, ENEMY_TEXTURE_ROW_IDLE, enemy->sprite.frames_per_line);
    }
    else
    {
        SetRow(&enemy->sprite.anim, ENEMY_TEXTURE_ROW_ANGRY, enemy->sprite.frames_per_line);
    }
}

void CreateWiz(Enemy* enemy)
{
    const auto enemy_anim = (Animation){
        .first = 0,
        .last = 0,
        .current = 0,
        .speed = 0.4f,
        .time_left = 0
    };

    const auto enemy_sprite = (Sprite){
        .texture = assets.spritesheet_dark_wiz,
        .frames_per_line = 4,
        .origin = {0, 0},
        .rotation = 0.0f,
        .anim = enemy_anim
    };

    *enemy = (Enemy){
        .type = WIZ,
        .sprite = enemy_sprite,
        .alive = true,
        .position = {300, 100},
        .hp = 100,
        .max_hp = 100,
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

void CreateEnemy(Enemy* enemy, const Type type)
{
    switch (type)
    {
    case RED_SLIME: CreateRedSlime(enemy);
        break;
    case WIZ: CreateWiz(enemy);
        break;
    }
}

void UpdateEnemy(Enemy* enemy, TileMap* map, Player* player)
{
    //Shared by all enemy types, so a dead enemy stops acting before its own update runs
    if (enemy->hp <= 0) enemy->alive = false;
    if (!enemy->alive) return;

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
    if (!enemy->alive) return;
    DrawSprite(&enemy->sprite, enemy->position);
}
