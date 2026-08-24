#include <stdlib.h>

#include "../include/tilemap.h"

void CreateEmptyLevel(TileMap* tilemap)
{
    tilemap->height = DEFAULT_HEIGHT;
    tilemap->width = DEFAULT_WIDTH;
    tilemap->tiles = calloc(1, GetTilesDataSize(tilemap));
}
