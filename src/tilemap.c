//
// Created by mori on 12.07.26.
//

#include <raylib.h>
#include "../include/tilemap.h"

void DestroyTileMap(TileMap *tilemap) {
}

void DrawTileMap(TileMap *tilemap)
{
    for (int y = 0; y < tilemap->height; y++)
    {
        for (int x = 0; x < tilemap->width; x++)
        {
            int tile_id = tilemap->tiles[y * tilemap->width + x];

            Rectangle source = {
                tile_id * TILE_SIZE,
                0,
                TILE_SIZE,
                TILE_SIZE
            };

            Rectangle dest = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            DrawTexturePro(
                tilemap->tileset,
                source,
                dest,
                (Vector2){0,0},
                0,
                WHITE
            );
        }
    }
}

bool CheckCollisionWithMap(TileMap *tilemap, Vector2 pos)
{
    float tile_x = pos.x / TILE_SIZE;
    float tile_y = pos.y / TILE_SIZE;

    // außerhalb der Map
    if (tile_x <  (-0.2f) || tile_x >= tilemap->width - 0.8f)
        return true;

    if (tile_y < (-0.6f) || tile_y >= tilemap->height - 1)
        return true;

    return false;
}