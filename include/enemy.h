#ifndef TESTGAME2_ENEMY_H
#define TESTGAME2_ENEMY_H
#include "sprite.h"
#include "tilemap.h"

#define ENEMY_HIT_RADIUS 10

#define ENEMY_RED_SLIME_HP 30
#define ENEMY_RED_SLIME_VELOCITY 15
#define ENEMY_RED_SLIME_DAMAGE 15
#define ENEMY_RED_SLIME_TRIGGER_DISTANCE 100
#define ENEMY_RED_SLIME_HIT_RATE 2

#define ENEMY_WIZ_HP 200
#define ENEMY_WIZ_VELOCITY 10
#define ENEMY_WIZ_DAMAGE 30
#define ENEMY_WIZ_TRIGGER_DISTANCE 100
#define ENEMY_WIZ_HIT_RATE 2.f

typedef struct Player Player;

typedef enum
{
    RED_SLIME,
    WIZ,
} Type;

typedef struct Enemy
{
    Type type;
    Sprite sprite;
    Vector2 position;
    bool alive;
    int hp;
    float velocity;
    bool angry;
    float trigger_radius;
    float hit_radius;
    float hit_rate;
    float hit_cooldown;
    int damage_per_hit;
} Enemy;

void CreateRedSlime(Enemy* enemy);
void UpdateRedSlime(Enemy* enemy, TileMap* map, Player* player);

void CreateWiz(Enemy* enemy);
void UpdateWiz(Enemy* enemy, TileMap* map, Player* player);

void UpdateEnemy(Enemy* enemy, TileMap* map, Player* player);
void DrawEnemy(Enemy* enemy);

#endif //TESTGAME2_ENEMY_H
