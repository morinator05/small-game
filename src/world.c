//
// Created by mori on 16.08.26.
//

#include "../include/world.h"
#include "../include/enemy.h"

void CreateWorld(World *world, const int id, const TileMap map)
{
    *world = (World) {
        .id = id,
        .map = map,
        .enemy_count = 0,
        .trigger_count = 0};
}

bool AddEnemyToWorld(World *world, const Enemy enemy)
{
    if (world->enemy_count >= MAX_ENEMIES) return false;
    world->enemies[world->enemy_count] = enemy;
    world->enemy_count ++;
    return true;
}
bool AddTriggerToWorld(World *world, Rectangle bounds, int target_world_id, Vector2 target_spawn)
{
    //TODO: implement triggers to switch worlds
}

void UpdateWorld(World *world, void *player)
{
    for (int i = 0; i < world->enemy_count; i++)
    {
        UpdateEnemy(&world->enemies[i], &world->map, player);
    }
}
void DrawWorld(World *world)
{
    DrawTileMap(&world->map);

    for (int i = 0; i < world->enemy_count; i++)
    {
        DrawEnemy(&world->enemies[i]);
    }
}