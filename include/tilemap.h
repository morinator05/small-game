//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_TILEMAP_H
#define TESTGAME2_TILEMAP_H
#include <raylib.h>

#include "assets.h"

#define TILE_SIZE 16

typedef enum
{
    TILE_BORDER = -1,
    TILE_GRASS = 1,
    TILE_SAND = 2,
    TILE_DIRT = 3,
    TILE_WATER = 4,

}TileCodes;

typedef enum
{
    OUTER_CORNER = 0,
    H_UPPER_LINE = 1,
    INNER_CORNER = 2,
    V_LEFT_LINE = 3,
    SOLID = 4,
}TileTypeIndex;

typedef struct
{
    int width;
    int height;
    int* tiles;

    Assets assets;
} TileMap;

void DrawTileMap(TileMap* tilemap);
void DestroyTileMap(TileMap* tilemap);
bool CheckCollisionWithMap(const TileMap* tilemap, Rectangle playerRect);
void UpdateEditor(const TileMap* map, Camera2D camera);

#endif //TESTGAME2_TILEMAP_H
