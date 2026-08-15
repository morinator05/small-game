//
// Created by mori on 15.08.26.
//

#ifndef TESTGAME2_ENEMY_H
#define TESTGAME2_ENEMY_H
#include "player.h"
#include "sprite.h"

#define   TEXTURE_ENEMY_ROW_IDLE 0
#define TEXTURE_ENEMY_ROW_ANGRY 1

typedef struct
{
    Sprite sprite;
    Vector2 position;
    int hp;
    int velocity;
    bool angry;
    float trigger_radius;
    float hit_radius;
    float hit_rate;
    float hit_cooldown;
    int damage_per_hit;
} Enemy;

void CreateRedSlime(Enemy* enemy, Texture2D texture);
void UpdateRedSlime(Enemy* enemy, TileMap* map, Player* player);

void CreateWiz(Enemy* enemy, Texture2D texture);
void UpdateWiz(Enemy* enemy, TileMap* map, Player* player);

void DrawEnemy(Enemy* enemy);

#endif //TESTGAME2_ENEMY_H
