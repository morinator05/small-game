#ifndef TESTGAME2_WORLD_H
#define TESTGAME2_WORLD_H
#include <raylib.h>

#include "enemy.h"
#include "tilemap.h"

#define MAX_ENEMIES 100
#define MAX_TRIGGERS 10
#define SPAWN_MAX_ATTEMPTS 1000
#define WORLD_NAME_LENGTH 32
#define WORLDS_DIR "asset/worlds"

typedef Enemy Enemy;

typedef struct
{
    Rectangle hitbox;
    int target_world_id;
    Vector2 target_spawn;
} WorldTrigger;

typedef struct World
{
    int id;
    char name[WORLD_NAME_LENGTH]; //folder in WORLDS_DIR, empty if the slot is unused
    TileMap tile_map;

    Enemy enemies[MAX_ENEMIES];
    int enemy_count;

    WorldTrigger triggers[MAX_TRIGGERS];
    int trigger_count;
} World;

//Loads the tile map from WORLDS_DIR/<name>/terrain.map, or creates an empty one if there is none
void CreateWorld(World* world, int id, const char* name);
void DestroyWorld(World* world);
void GetWorldMapPath(const World* world, char* out, size_t size);
bool AddEnemyToWorld(World* world, Enemy enemy);
//Places count enemies of the given type on random walkable tiles, returns how many were spawned
int SpawnRandomEnemies(World* world, Type type, int count);
bool AddTriggerToWorld(World* world, Rectangle bounds, int target_world_id, Vector2 target_spawn);

void UpdateWorld(World* world, void* player);
void DrawWorld(World* world);

#endif //TESTGAME2_WORLD_H
