//
// Created by mori on 16.08.26.
//

#ifndef TESTGAME2_WORLD_H
#define TESTGAME2_WORLD_H
#include "enemy.h"

#endif //TESTGAME2_WORLD_H

#define MAX_ENEMIES 100
#define MAX_TRIGGERS 10

typedef struct {
    Rectangle hitbox;
    int target_world_id;
    Vector2 target_spawn;
} WorldTrigger;

typedef struct {
    int id;
    TileMap map;

    Enemy enemies[MAX_ENEMIES];
    int enemy_count;

    WorldTrigger triggers[MAX_TRIGGERS];
    int trigger_count;
} World;

void CreateWorld(World *world, int id, TileMap map);
bool AddEnemyToWorld(World *world, Enemy enemy);
bool AddTriggerToWorld(World *world, Rectangle bounds, int target_world_id, Vector2 target_spawn);

void UpdateWorld(World *world, void *player);
void DrawWorld(World *world);