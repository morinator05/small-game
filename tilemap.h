//
// Created by mori on 12.07.26.
//

#ifndef TESTGAME2_TILEMAP_H
#define TESTGAME2_TILEMAP_H
#include <raylib.h>

typedef struct {
    int width;
    int height;
    int *tiles;

    Texture2D tileset;
} TileMap;

void CreateTileMap(TileMap *tilemap, int width, int height, int *tileset);
void DrawTileMap(TileMap *tilemap);
void DestroyTileMap(TileMap *tilemap);

#endif //TESTGAME2_TILEMAP_H