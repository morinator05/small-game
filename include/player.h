#ifndef TESTGAME2_PLAYER_H
#define TESTGAME2_PLAYER_H
#include "sprite.h"
#include "tilemap.h"

#define PLAYER_DEFAULT_HP 100
#define PLAYER_DEFAULT_VELOCITY 20
#define PLAYER_RUNNING_MULTIPLIER 2

typedef struct World World;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    IDLE,
} Directions;

typedef enum
{
    UNARMED,
    SWORD
}Weapons;

typedef struct Player
{
    Sprite sprite;
    Vector2 position;
    int velocity;
    int hp;
    int weapon_damage;
    float weapon_cooldown;
    float hit_rate;
    int weapon_RANGE;
    Directions current_direction;
} Player;

void CreatePlayer(Player* player, Texture2D texture);
void DrawPlayer(Player* player);
void MovePlayer(Player* player, const TileMap* tilemap);
void UpdatePlayer(Player* player, World* world);
void AquireWeapon(Player* player, Weapons weapon);

#endif //TESTGAME2_PLAYER_H
