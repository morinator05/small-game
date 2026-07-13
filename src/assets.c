//
// Created by mori on 12.07.26.
//
#include "assets.h"


void LoadAssets(Assets* assets) {
    assets->player =        LoadTexture("asset/player_idle_map.png");
    assets->worldDefault =  LoadTexture("asset/world_default.png");
}
void UnloadAssets(Assets* assets) {
    UnloadTexture(assets->player);
}