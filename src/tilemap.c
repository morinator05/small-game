#include <raylib.h>
#include "../include/tilemap.h"

#include <math.h>

#define PLAYER_MARGIN 4

typedef struct {
    TileTypeIndex index;
    float rotation;
} DualTileTransform;

static const DualTileTransform DUAL_GRID_LOOKUP[16] = {
    { SOLID, 0.0f },            // 00 00 -> nothing, gets skipped
    { OUTER_CORNER, 0.0f },     // 10 00 -> top left outer
    { OUTER_CORNER, 90.0f },    // 01 00 -> top right outer
    { H_UPPER_LINE, 0.0f },     // 11 00 -> top line horizontal

    { OUTER_CORNER, 270.0f },   // 00 10 -> bottom left outer
    { V_LEFT_LINE, 0.0f },      // 10 10 -> left line vertical
    { SOLID, 0.0f },            // 01 10 -> diagonal, TODO: this is bugged
    { INNER_CORNER, 0.0f },     // 11 10 -> bottom right inner

    { OUTER_CORNER, 180.0f },   // 00 01 -> bottom right outer
    { SOLID, 0.0f },            // 10 01 -> diagonal, TODO: also bugged
    { V_LEFT_LINE, 180.0f },    // 01 01 -> vertical line right
    { INNER_CORNER, 90.0f },    // 11 01 -> bottom left inner

    { H_UPPER_LINE, 180.0f },   // 00 11 -> horizontal line bottom
    { INNER_CORNER, 270.0f },   // 10 11 -> top right inner
    { INNER_CORNER, 180.0f },   // 01 11 -> top left inner
    { SOLID, 0.0f }             // 11 11 -> solid
};

void DestroyTileMap(TileMap* tilemap)
{
    //TODO
}

static int GetTileBit(const TileMap* tilemap, const int x, const int y, const TileCodes targetType)
{
    if (x < 0 || x >= tilemap->width || y < 0 || y >= tilemap->height)
    {
        return 0;
    }
    return tilemap->tiles[x + y * tilemap->width] == targetType ? 1 : 0;
}

static void DrawTileLayer(const TileMap* tilemap, const TileCodes tile_type_to_draw, const Texture2D texture)
{
    for (int y = 0; y <= tilemap->height; y++)
    {
        for (int x = 0; x <= tilemap->width; x++)
        {
            int top_left     = GetTileBit(tilemap, x - 1, y - 1, tile_type_to_draw);
            int top_right    = GetTileBit(tilemap, x,     y - 1, tile_type_to_draw);
            int bottom_left  = GetTileBit(tilemap, x - 1, y,     tile_type_to_draw);
            int bottom_right = GetTileBit(tilemap, x,     y,     tile_type_to_draw);

            int mask = top_left + (top_right << 1) + (bottom_left << 2) + (bottom_right << 3);
            if (mask == 0) continue;

            DualTileTransform t = DUAL_GRID_LOOKUP[mask];

            Rectangle src_rec = {
                (float)(t.index * TILE_SIZE),
                0.0f,
                (float)TILE_SIZE,
                (float)TILE_SIZE
            };

            const Rectangle dest_rec = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                (float)TILE_SIZE,
                (float)TILE_SIZE
            };

            Vector2 origin = { TILE_SIZE / 2.0f, TILE_SIZE / 2.0f };

            DrawTexturePro(
                texture,
                src_rec,
                dest_rec,
                origin,
                t.rotation,
                WHITE
            );
        }
    }
}

void DrawTileMap(TileMap* tilemap)
{
    for (int y = 0; y < tilemap->height; y++)
    {
        for (int x = 0; x < tilemap->width; x++)
        {
            const Rectangle grassTile = {
                SOLID * TILE_SIZE,
                0,
                TILE_SIZE,
                TILE_SIZE
            };
            DrawTextureRec(
                tilemap->assets.grasTiles,
                grassTile,
                (Vector2){ x * TILE_SIZE, y * TILE_SIZE },
                WHITE
            );
        }
    }

    DrawTileLayer(tilemap, TILE_DIRT, tilemap->assets.dirtTiles);
    DrawTileLayer(tilemap, TILE_WATER, tilemap->assets.waterTiles);

}


bool CheckCollisionWithMap(const TileMap* tilemap, const Rectangle playerRect)
{
    for (int y = 0; y < tilemap->height; y++)
    {
        for (int x = 0; x < tilemap->width; x++)
        {
            const int tile_code = tilemap->tiles[y * tilemap->width + x];
            if (tile_code != TILE_WATER &&
                tile_code != TILE_BORDER)
                continue;

            const Rectangle tile_rect = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };
            const Rectangle player_collision = {
                playerRect.x + PLAYER_MARGIN,
                playerRect.y + PLAYER_MARGIN,
                playerRect.width - 2 * PLAYER_MARGIN,
                playerRect.height - PLAYER_MARGIN
            };

            if (CheckCollisionRecs(player_collision, tile_rect))
                return true;
        }
    }
    return false;
}

//very basic editor
void UpdateEditor(const TileMap* map, const Camera2D camera)
{
    Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

    int tile_x = (int)floorf(mouse_world_pos.x / TILE_SIZE);
    int tile_y = (int)floor(mouse_world_pos.y / TILE_SIZE);

    if (tile_x >= 0 && tile_x < map->width && tile_y >= 0 && tile_y < map->height)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            map->tiles[tile_y * map->width + tile_x] = TILE_DIRT;
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            map->tiles[tile_y * map->width + tile_x] = TILE_GRASS;
        }
    }
}
