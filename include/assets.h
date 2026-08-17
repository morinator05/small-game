#ifndef TESTGAME2_ASSETS_H
#define TESTGAME2_ASSETS_H
#include <raylib.h>

#define FRAMES_PER_LINE 4

typedef struct
{
    Texture2D player;
    Texture2D waterTiles;
    Texture2D dirtTiles;
    Texture2D grasTiles;
    Texture2D redSlime;
    Texture2D darkWiz;
} Assets;

void LoadAssets(Assets* assets);
void UnloadAssets(const Assets* assets);

#endif //TESTGAME2_ASSETS_H
