//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_PLAYER_H
#define TESTGAME2_PLAYER_H
#include "sprite.h"


typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
}Direction;

typedef struct {
    Sprite sprite;
    Vector2 position;
    int velocity;
    Rectangle frameRectangle;
} Player;

void CreatePlayer(Player* player, Texture2D texture);
void DrawPlayer(Player* player);
void MovePlayer(Player* player);
void SetDirection(Player* player, Direction direction);

#endif //TESTGAME2_PLAYER_H