#ifndef TESTGAME2_ASSETS_H
#define TESTGAME2_ASSETS_H
#include <raylib.h>

#define FRAMES_PER_LINE 4

typedef struct
{
    //Tilesets
    Texture2D tileset_water;
    Texture2D tileset_dirt;
    Texture2D tileset_gras;

    //Spritesheets
    Texture2D spritesheet_player;
    Texture2D spritesheet_red_slime;
    Texture2D spritesheet_dark_wiz;

    //Sounds
    Sound player_walk_gras;
    Sound player_walk_dirt;
    Sound player_walk_sand;

    Sound slime_walk;
    Sound slime_attack;
} Assets;

void LoadAssets(Assets* assets);
void UnloadAssets(const Assets* assets);

#endif //TESTGAME2_ASSETS_H
