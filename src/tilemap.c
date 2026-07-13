//
// Created by mori on 12.07.26.
//

#include <raylib.h>
#include "tilemap.h"

void DestroyTileMap(TileMap *tilemap) {

}

void DrawTileMap(TileMap *tilemap)
{
    const int TILE_SIZE = 16;

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

bool CheckCollisionWithMap(TileMap *tilemap, Vector2 pos) {

}