//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_TILEMAP_H
#define TESTGAME2_TILEMAP_H
#include <raylib.h>

#define TILE_SIZE 16

#define TILE_CODE_BORDER (-1)
#define TILE_CODE_DIRT 0
#define TILE_CODE_WATER 1
#define TILE_CODE_SAND 2
#define TILE_CODE_GRASS 3


typedef struct {
    int width;
    int height;
    int *tiles;

    Texture2D tileset;
} TileMap;

void DrawTileMap(TileMap *tilemap);
void DestroyTileMap(TileMap *tilemap);
bool CheckCollisionWithMap(TileMap *tilemap, Rectangle playerRect);

#endif //TESTGAME2_TILEMAP_H
