#ifndef TESTGAME2_GAME_H
#define TESTGAME2_GAME_H
#include "player.h"
#include "world.h"

#define MAX_WORLDS 10

typedef enum
{
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING,
    GAME_STATE_EDITOR,
    GAME_STATE_PAUSED,
    GAME_STATE_MENU,
} GameState;

typedef struct
{
    GameState state;
    int current_world_idx;
    World worlds[MAX_WORLDS];
    Player player;
    Camera2D camera;
    int menu_selection;
    bool has_save; //decides if the title menu offers Continue
    bool quit;
} Game;

void CreateGame(Game* game);
void DestroyGame(Game* game);
void UpdateGame(Game* game);
void DrawGame(Game* game);

#endif //TESTGAME2_GAME_H
