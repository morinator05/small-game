//
// Created by mori on 12.07.26.
//

#include <raylib.h>
#include "../include/tilemap.h"

#include <stdio.h>

#define PLAYER_MARGIN 4

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

bool CheckCollisionWithMap(TileMap *tilemap, Rectangle playerRect)
{
    for (int y = 0; y < tilemap->height; y++)
    {
        for (int x = 0; x < tilemap->width; x++)
        {
            if (tilemap->tiles[y * tilemap->width + x] != 1)
                continue;

            Rectangle tileRect = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            Rectangle playerCollision = {
                playerRect.x + PLAYER_MARGIN,
                playerRect.y + PLAYER_MARGIN,
                playerRect.width - (2 * PLAYER_MARGIN),
                playerRect.height - PLAYER_MARGIN
            };

            if (CheckCollisionRecs(playerCollision, tileRect))
                return true;
        }
    }

    return false;
}