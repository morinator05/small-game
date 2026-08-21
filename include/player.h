#ifndef TESTGAME2_PLAYER_H
#define TESTGAME2_PLAYER_H
#include "sprite.h"
#include "tilemap.h"

#define PLAYER_DEFAULT_HP 100
#define PLAYER_DEFAULT_VELOCITY 20
#define PLAYER_RUNNING_MULTIPLIER 2

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    IDLE,
} Direction;

typedef struct
{
    Sprite sprite;
    Vector2 position;
    int velocity;
    float hp;
    Direction current_direction;
} Player;

void CreatePlayer(Player* player, Texture2D texture);
void DrawPlayer(Player* player);
void MovePlayer(Player* player, const TileMap* tilemap);

#endif //TESTGAME2_PLAYER_H
