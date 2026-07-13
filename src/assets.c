//
// Created by mori on 12.07.26.
//
#include "../include/assets.h"


void LoadAssets(Assets* assets) {
    assets->player =        LoadTexture("asset/player_all_tiles.png");
    assets->worldDefault =  LoadTexture("asset/world_default.png");
}
void UnloadAssets(Assets* assets) {
    UnloadTexture(assets->player);
    UnloadTexture(assets->worldDefault);
}