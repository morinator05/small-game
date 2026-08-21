#ifndef TESTGAME2_ENEMY_H
#define TESTGAME2_ENEMY_H
#include "player.h"
#include "sprite.h"

#define ENEMY_HIT_RADIUS 10

#define ENEMY_RED_SLIME_HP 30
#define ENEMY_RED_SLIME_VELOCITY 24
#define ENEMY_RED_SLIME_DAMAGE 15
#define ENEMY_RED_SLIME_TRIGGER_DISTANCE 100
#define ENEMY_RED_SLIME_HIT_RATE 2

#define ENEMY_WIZ_HP 200
#define ENEMY_WIZ_VELOCITY 10
#define ENEMY_WIZ_DAMAGE 30
#define ENEMY_WIZ_TRIGGER_DISTANCE 100
#define ENEMY_WIZ_HIT_RATE 2.f

typedef enum
{
    RED_SLIME,
    WIZ,
} Type;

typedef struct
{
    Type type;
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

void UpdateEnemy(Enemy* enemy, TileMap* map, Player* player);
void DrawEnemy(Enemy* enemy);

#endif //TESTGAME2_ENEMY_H
