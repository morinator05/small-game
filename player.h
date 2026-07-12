//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_PLAYER_H
#define TESTGAME2_PLAYER_H

#include "sprite.h"

typedef struct {
    Sprite sprite;
    float velocity;
} Player;

void DrawPlayer(Player* player);
void MovePlayer(Player* player);

#endif //TESTGAME2_PLAYER_H