//
// Created by mori on 13.07.26.
//

#include "../include/assets.h"
#include "../include/tilemap.h"

static int mapLevel1[] = {
    0,0,0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
};

void CreateLevel1(TileMap* tilemap, Assets assets) {
    tilemap->height = 5;
    tilemap->width = 10;
    tilemap->tiles = mapLevel1;
    tilemap->tileset = assets.worldDefault;
}