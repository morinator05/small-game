#include "../include/world.h"
#include "../include/enemy.h"
#include "../include/assets.h"

void CreateWorld(World* world, const int id, const TileMap tile_map)
{
    *world = (World){
        .id = id,
        .tile_map = tile_map,
        .enemy_count = 0,
        .trigger_count = 0
    };
}

bool AddEnemyToWorld(World* world, const Enemy enemy)
{
    if (world->enemy_count >= MAX_ENEMIES) return false;
    world->enemies[world->enemy_count] = enemy;
    world->enemy_count++;
    return true;
}

static bool IsSpawnableTile(const TileMap* tile_map, const int x, const int y)
{
    const int tile_code = tile_map->tiles[y * tile_map->width + x];
    if (tile_code != TILE_GRASS && tile_code != TILE_SAND && tile_code != TILE_DIRT) return false;

    //Neighbouring water or border can still overlap the tile, so check the actual hitbox
    const Rectangle rect = {(float)x * TILE_SIZE, (float)y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    return !CheckCollisionWithMap(tile_map, rect);
}

int SpawnRandomEnemies(World* world, const Type type, const int count)
{
    const TileMap* tile_map = &world->tile_map;
    if (tile_map->tiles == NULL || tile_map->width <= 0 || tile_map->height <= 0) return 0;

    int spawned = 0;
    //Give up after a while so a map without free tiles can't hang the game
    for (int attempt = 0; attempt < SPAWN_MAX_ATTEMPTS && spawned < count; attempt++)
    {
        const int x = GetRandomValue(0, tile_map->width - 1);
        const int y = GetRandomValue(0, tile_map->height - 1);
        if (!IsSpawnableTile(tile_map, x, y)) continue;

        Enemy enemy;
        CreateEnemy(&enemy, type);
        enemy.position = (Vector2){(float)x * TILE_SIZE, (float)y * TILE_SIZE};
        if (!AddEnemyToWorld(world, enemy)) break;
        spawned++;
    }
    return spawned;
}

bool AddTriggerToWorld(World* world, Rectangle bounds, int target_world_id, Vector2 target_spawn)
{
    //TODO: implement triggers to switch worlds
    return false;
}

void UpdateWorld(World* world, void* player)
{
    for (int i = 0; i < world->enemy_count; i++)
    {
        UpdateEnemy(&world->enemies[i], &world->tile_map, player);
    }
}

void DrawWorld(World* world)
{
    DrawTileMap(&world->tile_map);

    for (int i = 0; i < world->enemy_count; i++)
    {
        DrawEnemy(&world->enemies[i]);
    }
}
