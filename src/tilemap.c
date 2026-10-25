#include <raylib.h>
#include "../include/tilemap.h"

#include <errno.h>

#include "../include/assets.h"
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void DestroyTileMap(const TileMap* tile_map)
{
    free(tile_map->tiles);
}

//Returns 1 if the Tile at the given position is of equal type with the given type.
static int GetTileBit(const TileMap* tile_map, const int x, const int y, const TileCodes targetType)
{
    if (x < 0 || x >= tile_map->width || y < 0 || y >= tile_map->height)
    {
        return 0;
    }
    return tile_map->tiles[x + y * tile_map->width] == targetType ? 1 : 0;
}

static void DrawTileLayer(const TileMap* tilemap, const TileCodes tile_type_to_draw, const Texture2D texture)
{
    for (int y = 0; y < tilemap->height; y++)
    {
        for (int x = 0; x < tilemap->width; x++)
        {
            const int top_left = GetTileBit(tilemap, x - 1, y - 1, tile_type_to_draw);
            const int top_right = GetTileBit(tilemap, x, y - 1, tile_type_to_draw);
            const int bottom_left = GetTileBit(tilemap, x - 1, y, tile_type_to_draw);
            const int bottom_right = GetTileBit(tilemap, x, y, tile_type_to_draw);

            //create a mask consisting of the bits of each 4 adjacent tiles
            const int mask = top_left + (top_right << 1) + (bottom_left << 2) + (bottom_right << 3);

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
                (int)x * TILE_SIZE,
                (int)y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            //Set the origin for the rotation to the center of the tile
            //Also shifts the tile to its intended position in the dual grid
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

void DrawTileMapDebugGrid(const TileMap* tile_map)
{
    for (int y = 0; y < tile_map->height; y++)
    {
        for (int x = 0; x < tile_map->width; x++)
        {
            const int pos_x = x * TILE_SIZE;
            const int pos_y = y * TILE_SIZE;

            DrawRectangleLines(pos_x, pos_y, TILE_SIZE, TILE_SIZE, RED);
        }
    }
}

void DrawTileMap(const TileMap* tile_map)
{
    for (int y = 0; y < tile_map->height; y++)
    {
        for (int x = 0; x < tile_map->width; x++)
        {
            if (tile_map->tiles[y * tile_map->width + x] == 0) continue;
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
    DrawTileLayer(tile_map, TILE_SAND, assets.tileset_sand);
    DrawTileLayer(tile_map, TILE_DIRT, assets.tileset_dirt);
    DrawTileLayer(tile_map, TILE_WATER, assets.tileset_water);
    DrawTileLayer(tile_map, TILE_BORDER, assets.tileset_border);
}


bool CheckCollisionWithMap(const TileMap* tile_map, const Rectangle player_rect)
{
    for (int y = 0; y < tile_map->height; y++)
    {
        for (int x = 0; x < tile_map->width; x++)
        {
            const int tile_code = tile_map->tiles[y * tile_map->width + x];
            if (tile_code != TILE_WATER &&
                tile_code != TILE_BORDER)
                continue;

            const Rectangle tile_rect = {
                (float)x * TILE_SIZE,
                (float)y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };
            const Rectangle player_collision = {
                player_rect.x + PLAYER_MARGIN,
                player_rect.y + PLAYER_MARGIN,
                player_rect.width - 2 * PLAYER_MARGIN,
                player_rect.height - PLAYER_MARGIN
            };

            if (CheckCollisionRecs(player_collision, tile_rect))
                return true;
        }
    }
    return false;
}

//very basic editor
void UpdateEditor(const TileMap* tile_map, const Camera2D camera)
{
    const Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

    const int tile_x = (int)floorf(mouse_world_pos.x / TILE_SIZE);
    const int tile_y = (int)floor(mouse_world_pos.y / TILE_SIZE);

    if (tile_x >= 0 && tile_x < tile_map->width && tile_y >= 0 && tile_y < tile_map->height)
    {
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                tile_map->tiles[tile_y * tile_map->width + tile_x] = TILE_WATER;
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                tile_map->tiles[tile_y * tile_map->width + tile_x] = TILE_SAND;
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
            {
                tile_map->tiles[tile_y * tile_map->width + tile_x] = TILE_VOID;
            }
        }
        else
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                tile_map->tiles[tile_y * tile_map->width + tile_x] = TILE_DIRT;
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                tile_map->tiles[tile_y * tile_map->width + tile_x] = TILE_GRASS;
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
            {
                tile_map->tiles[tile_y * tile_map->width + tile_x] = TILE_BORDER;
            }
        }
    }
}

void LoadTileMap(TileMap* tile_map, const char* file)
{
    const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    const int fd = open(file, O_RDONLY, mode);
    if (fd < 0)
    {
        printf("Can't open file for reading: %s\n", strerror(errno));
        return;
    }

    //Read map size
    if (read(fd, &tile_map->width, sizeof(int)) != sizeof(int) || read(fd, &tile_map->height, sizeof(int)) != sizeof(
        int))
    {
        printf("Error reading map size: %s\n", strerror(errno));
    }

    size_t bytes_to_read = GetTilesDataSize(tile_map);
    free(tile_map->tiles);
    tile_map->tiles = calloc(1, bytes_to_read);
    size_t bytes_read = read(fd, tile_map->tiles, bytes_to_read);
    if (bytes_read != bytes_to_read)
    {
        printf("Error reading map data: %s\n", strerror(errno));
    }

    close(fd);
    printf("Loaded %d tiles\n", tile_map->width * tile_map->height);
}

void SaveTileMap(const TileMap* tile_map, const char* file)
{
    printf("Starting writing tilemap: %s\n", file);

    const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    const int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (fd < 0)
    {
        printf("Error opening file for writing: %s\n", strerror(errno));
        return;
    }

    //Write header
    if (write(fd, &tile_map->width, sizeof(int)) != sizeof(int)
        || write(fd, &tile_map->height, sizeof(int)) != sizeof(int))
    {
        printf("Error writing map size: %s\n", strerror(errno));
    }

    //Write map data
    size_t bytes_to_write = GetTilesDataSize(tile_map);
    size_t bytes_written = write(fd, tile_map->tiles, bytes_to_write);
    if (bytes_written != bytes_to_write)
    {
        printf("Error writing tilemap data: %s\n", strerror(errno));
    }

    close(fd);
    printf("Finished writing tilemap.\n");
}
