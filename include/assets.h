#ifndef TESTGAME2_ASSETS_H
#define TESTGAME2_ASSETS_H
#include <raylib.h>

#define ANIM_FRAMES_PER_LINE 4
#define ANIM_FRAME_FIRST 0
#define ANIM_FRAME_LAST 3
#define ANIM_SPEED .2f
#define TILE_SIZE 16

#define PLAYER_TEXTURE_ROW_IDLE 0
#define PLAYER_TEXTURE_ROW_DOWN 1
#define PLAYER_TEXTURE_ROW_UP 2
#define PLAYER_TEXTURE_ROW_LEFT 3
#define PLAYER_TEXTURE_ROW_RIGHT 4

#define ENEMY_TEXTURE_ROW_IDLE 0
#define ENEMY_TEXTURE_ROW_ANGRY 1

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
    Texture2D spritesheet_sword;

    //Ambient sounds
    Sound ambient_forrest;

    //Walking
    Sound slime_walk;
    Sound slime_attack;
    Sound player_walk_gras;
    Sound player_walk_dirt;
    Sound player_walk_sand;
} Assets;

extern Assets assets;

void LoadAssets();
void UnloadAssets();

#endif //TESTGAME2_ASSETS_H
