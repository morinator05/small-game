#include "../include/assets.h"

Assets assets;

void LoadAssets()
{
    assets.spritesheet_player = LoadTexture("asset/sprites/player.png");
    assets.tileset_gras = LoadTexture("asset/tiles/gras.png");
    assets.tileset_dirt = LoadTexture("asset/tiles/dirt.png");
    assets.tileset_water = LoadTexture("asset/tiles/water.png");
    assets.tileset_sand = LoadTexture("asset/tiles/sand.png");
    assets.tileset_border = LoadTexture("asset/tiles/border.png");
    assets.spritesheet_red_slime = LoadTexture("asset/sprites/slime_red.png");
    assets.spritesheet_dark_wiz = LoadTexture("asset/sprites/purple_wizard.png");
    assets.spritesheet_sword = LoadTexture("asset/weapons/sword.png");

    assets.player_walk_dirt = LoadSound("asset/sounds/walking/dirt.mp3");
    assets.player_walk_gras = LoadSound("asset/sounds/walking/grass.mp3");
    assets.ambient_forrest = LoadSound("asset/sounds/ambient/forrest.mp3");

    assets.sfx_sword = LoadSound("asset/sounds/sfx/sword.mp3");
    assets.sfx_hit = LoadSound("asset/sounds/sfx/hit.mp3");
}

void UnloadAssets()
{
    UnloadTexture(assets.spritesheet_player);
    UnloadTexture(assets.tileset_dirt);
    UnloadTexture(assets.tileset_water);
    UnloadTexture(assets.tileset_gras);
    UnloadTexture(assets.tileset_sand);
    UnloadTexture(assets.tileset_border);
    UnloadTexture(assets.spritesheet_red_slime);
    UnloadTexture(assets.spritesheet_dark_wiz);
    UnloadTexture(assets.spritesheet_sword);

    UnloadSound(assets.player_walk_dirt);
    UnloadSound(assets.player_walk_gras);
    UnloadSound(assets.ambient_forrest);

    UnloadSound(assets.sfx_sword);
    UnloadSound(assets.sfx_hit);
}
