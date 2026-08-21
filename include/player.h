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
    SWORD,
    BOW
}WeaponTypes;

typedef struct
{
    WeaponTypes type;
    int damage;
    float hit_rate;
    float cooldown;
    float reach;
}Weapon;

typedef struct Player
{
    Sprite sprite;
    Vector2 position;
    int velocity;
    int hp;
    Weapon weapon;
    Directions current_direction;
} Player;

void CreatePlayer(Player* player);
void DrawPlayer(Player* player);
void MovePlayer(Player* player, const TileMap* tilemap);
void UpdatePlayer(Player* player, World* world);
void AcquireWeapon(Player* player, WeaponTypes weapon_type);

#endif //TESTGAME2_PLAYER_H
