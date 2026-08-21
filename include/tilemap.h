#ifndef TESTGAME2_TILEMAP_H
#define TESTGAME2_TILEMAP_H
#include <raylib.h>

#include "assets.h"

#define PLAYER_MARGIN 4

//Tile identifiers in the actual game map
typedef enum
{
    TILE_BORDER = -1,
    TILE_GRASS = 1,
    TILE_SAND = 2,
    TILE_DIRT = 3,
    TILE_WATER = 4,
} TileCodes;


typedef enum
{
    OUTER_CORNER = 0,
    H_UPPER_LINE = 1,
    INNER_CORNER = 2,
    V_LEFT_LINE = 3,
    SOLID = 4,
} TileTypeIndex;

typedef struct
{
    TileTypeIndex index; //which one of the tiles
    float rotation; //the rotation to fit
} DualTileTransform;

//We are only checking the 4 adjacent Tiles of the position where 4 tiles join, stored in 4 bits -> 0-15
//This maps every possible combination. Every bit represents a corner surrounding the point.
static const DualTileTransform DUAL_GRID_LOOKUP[16] = {
    {SOLID, 0.0f}, // 00 00 -> nothing, gets skipped
    {OUTER_CORNER, 0.0f}, // 10 00 -> top left outer
    {OUTER_CORNER, 90.0f}, // 01 00 -> top right outer
    {H_UPPER_LINE, 0.0f}, // 11 00 -> top line horizontal

    {OUTER_CORNER, 270.0f}, // 00 10 -> bottom left outer
    {V_LEFT_LINE, 0.0f}, // 10 10 -> left line vertical
    {SOLID, 0.0f}, // 01 10 -> diagonal, TODO: this is bugged
    {INNER_CORNER, 0.0f}, // 11 10 -> bottom right inner

    {OUTER_CORNER, 180.0f}, // 00 01 -> bottom right outer
    {SOLID, 0.0f}, // 10 01 -> diagonal, TODO: also bugged
    {V_LEFT_LINE, 180.0f}, // 01 01 -> vertical line right
    {INNER_CORNER, 90.0f}, // 11 01 -> bottom left inner

    {H_UPPER_LINE, 180.0f}, // 00 11 -> horizontal line bottom
    {INNER_CORNER, 270.0f}, // 10 11 -> top right inner
    {INNER_CORNER, 180.0f}, // 01 11 -> top left inner
    {SOLID, 0.0f} // 11 11 -> solid
};

typedef struct
{
    int width;
    int height;
    int* tiles;

    Assets assets;
} TileMap;

void DrawTileMap(const TileMap* tilemap);
void DestroyTileMap(TileMap* tilemap);
bool CheckCollisionWithMap(const TileMap* tilemap, Rectangle playerRect);
void UpdateEditor(const TileMap* map, Camera2D camera);

#endif //TESTGAME2_TILEMAP_H
