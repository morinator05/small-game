//
// Created by mori on 15.08.26.
//

#ifndef TESTGAME2_ENEMY_H
#define TESTGAME2_ENEMY_H
#include "player.h"
#include "sprite.h"

typedef struct
{
    Sprite sprite;
    Vector2 position;
    int hp;
    int velocity;
    bool angry;
    float trigger_radius;
}Enemy;

void CreateEnemy(Enemy* enemy, Texture2D texture);
void UpdateEnemy(Enemy* enemy, TileMap* map, const Player* player);
void DrawEnemy(Enemy* enemy);

#endif //TESTGAME2_ENEMY_H
