//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_ASSETS_H
#define TESTGAME2_ASSETS_H
#include <raylib.h>

typedef struct
{
    Texture2D player;
    Texture2D worldDefault;
} Assets;

void LoadAssets(Assets* assets);
void UnloadAssets(Assets* assets);

#endif //TESTGAME2_ASSETS_H
