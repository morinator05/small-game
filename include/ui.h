#ifndef TESTGAME2_UI_H
#define TESTGAME2_UI_H
#include <raylib.h>

#include "player.h"
#include "world.h"

typedef struct
{
    const char* title; //can be NULL
    const char* const* items;
    int count;
    float center_x; //relative to the screen, 0.5 is centered
    float center_y;
} Menu;

//Returns the index of the activated item or -1
int UpdateMenu(const Menu* menu, int* selected);
void DrawMenu(const Menu* menu, int selected);

//Screen space
void DrawTitleScreen(void);
void DrawHud(const Player* player, const World* world);
void DrawPauseScreen(void);
void DrawGameMenu(const Menu* menu, int selected, const Player* player);
void DrawEditorUi(const TileMap* tile_map, Camera2D camera);

//World space, call between BeginMode2D and EndMode2D
void DrawEnemyHealthBars(const World* world);
void DrawEditorCursor(const TileMap* tile_map, Camera2D camera);

#endif //TESTGAME2_UI_H
