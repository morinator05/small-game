#include <raylib.h>
#include "../include/tilemap.h"
#include "../include/assets.h"
#include <math.h>

void DestroyTileMap(TileMap* tilemap)
{
    //TODO
}

//Returns 1 if the Tile at the given position is of equal type with the given type.
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
            int top_left = GetTileBit(tilemap, x - 1, y - 1, tile_type_to_draw);
            int top_right = GetTileBit(tilemap, x, y - 1, tile_type_to_draw);
            int bottom_left = GetTileBit(tilemap, x - 1, y, tile_type_to_draw);
            int bottom_right = GetTileBit(tilemap, x, y, tile_type_to_draw);

            //create a mask consisting of the bits of each 4 adjacent tiles
            int mask = top_left + (top_right << 1) + (bottom_left << 2) + (bottom_right << 3);

            //nothing to draw there, goto next
            if (mask == 0) continue;

            //lookup the correct Tile and Rotation
            const DualTileTransform t = DUAL_GRID_LOOKUP[mask];

            //select the correct tile by its index in the tilemap and the coordinate to draw the tile
            const Rectangle src_rec = {
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

            //Set the origin for the rotation to the center of the tile
            //Also shifts the tile to its intendet position in the dual grid
            const Vector2 origin = {TILE_SIZE / 2.0f, TILE_SIZE / 2.0f};

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

void DrawTileMapDebugGrid(const TileMap* tilemap)
{
    for (int y = 0; y < tilemap->height; y++)
    {
        for (int x = 0; x < tilemap->width; x++)
        {
            int pos_x = x * TILE_SIZE;
            int pos_y = y * TILE_SIZE;

            DrawRectangleLines(pos_x, pos_y, TILE_SIZE, TILE_SIZE, RED);
        }
    }
}

void DrawTileMap(const TileMap* tilemap)
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
                assets.tileset_gras,
                grassTile,
                (Vector2){x * TILE_SIZE, y * TILE_SIZE},
                WHITE
            );
        }
    }

    //Draw the layers from dirt -> water -> ...coming soon
    DrawTileLayer(tilemap, TILE_SAND, assets.tileset_sand);
    DrawTileLayer(tilemap, TILE_DIRT, assets.tileset_dirt);
    DrawTileLayer(tilemap, TILE_BORDER, assets.tileset_border);
    DrawTileLayer(tilemap, TILE_WATER, assets.tileset_water);
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
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsKeyDown(KEY_LEFT_SHIFT))
        {
            map->tiles[tile_y * map->width + tile_x] = TILE_DIRT;
            PlaySound(assets.player_walk_dirt);
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !IsKeyDown(KEY_LEFT_SHIFT))
        {
            map->tiles[tile_y * map->width + tile_x] = TILE_GRASS;
            PlaySound(assets.player_walk_gras);
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT))
        {
            map->tiles[tile_y * map->width + tile_x] = TILE_WATER;
        }
    }
}
