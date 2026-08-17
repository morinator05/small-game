#include "../include/assets.h"


void LoadAssets(Assets* assets)
{
    assets->player = LoadTexture("asset/player_all_tiles.png");
    assets->grasTiles = LoadTexture("asset/tiles/gras.png");
    assets->dirtTiles = LoadTexture("asset/tiles/dirt.png");
    assets->waterTiles = LoadTexture("asset/tiles/water.png");
    assets->redSlime = LoadTexture("asset/slime_red.png");
    assets->darkWiz = LoadTexture("asset/purple_wizard.png");
}

void UnloadAssets(const Assets* assets)
{
    UnloadTexture(assets->player);
    UnloadTexture(assets->dirtTiles);
    UnloadTexture(assets->waterTiles);
    UnloadTexture(assets->grasTiles);
    UnloadTexture(assets->redSlime);
    UnloadTexture(assets->redSlime);
}
