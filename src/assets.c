#include "../include/assets.h"


void LoadAssets(Assets* assets)
{
    assets->spritesheet_player = LoadTexture("asset/player_all_tiles.png");
    assets->tileset_gras = LoadTexture("asset/tiles/gras.png");
    assets->tileset_dirt = LoadTexture("asset/tiles/dirt.png");
    assets->tileset_water = LoadTexture("asset/tiles/water.png");
    assets->spritesheet_red_slime = LoadTexture("asset/slime_red.png");
    assets->spritesheet_dark_wiz = LoadTexture("asset/purple_wizard.png");
}

void UnloadAssets(const Assets* assets)
{
    UnloadTexture(assets->spritesheet_player);
    UnloadTexture(assets->tileset_dirt);
    UnloadTexture(assets->tileset_water);
    UnloadTexture(assets->tileset_gras);
    UnloadTexture(assets->spritesheet_red_slime);
    UnloadTexture(assets->spritesheet_dark_wiz);
}
