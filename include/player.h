//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_PLAYER_H
#define TESTGAME2_PLAYER_H
#include "sprite.h"
#include "tilemap.h"

#define   TEXTURE_PLAYER_ROW_IDLE 0
#define TEXTURE_PLAYER_ROW_DOWN 1
#define TEXTURE_PLAYER_ROW_UP 2
#define TEXTURE_PLAYER_ROW_LEFT 3
#define TEXTURE_PLAYER_ROW_RIGHT 4

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
    Direction current_direction;
} Player;

void CreatePlayer(Player* player, Texture2D texture);
void DrawPlayer(Player* player);
void MovePlayer(Player* player, TileMap* tilemap);

#endif //TESTGAME2_PLAYER_H
