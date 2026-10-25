#include "../include/game.h"

#include <raymath.h>
#include <stdlib.h>
#include <string.h>

#include "../include/assets.h"
#include "../include/levels.h"
#include "../include/player.h"
#include "../include/ui.h"

#define DEFAULT_MAP_PATH "asset/maps/default_map.map"

typedef enum
{
    TITLE_MENU_START,
    TITLE_MENU_QUIT,
    TITLE_MENU_COUNT,
} TitleMenuItem;

static const char* const TITLE_MENU_ITEMS[TITLE_MENU_COUNT] = {
    [TITLE_MENU_START] = "Start Game",
    [TITLE_MENU_QUIT] = "Quit",
};

static const Menu TITLE_MENU = {
    .title = NULL,
    .items = TITLE_MENU_ITEMS,
    .item_count = TITLE_MENU_COUNT,
};

typedef enum
{
    GAME_MENU_RESUME,
    GAME_MENU_TITLE,
    GAME_MENU_QUIT,
    GAME_MENU_COUNT,
} GameMenuItem;

static const char* const GAME_MENU_ITEMS[GAME_MENU_COUNT] = {
    [GAME_MENU_RESUME] = "Resume",
    [GAME_MENU_TITLE] = "Back to Title",
    [GAME_MENU_QUIT] = "Quit Game",
};

static const Menu GAME_MENU = {
    .title = "Menu",
    .items = GAME_MENU_ITEMS,
    .item_count = GAME_MENU_COUNT,
};

void CreateGame(Game* game)
{
    memset(game, 0, sizeof(*game));
    game->state = GAME_STATE_TITLE;

    TileMap default_tile_map = {0};
    LoadTileMap(&default_tile_map, DEFAULT_MAP_PATH);
    if (default_tile_map.tiles == NULL) CreateEmptyLevel(&default_tile_map);
    CreateWorld(&game->worlds[0], 0, default_tile_map);
    game->current_world_idx = 0;

    CreatePlayer(&game->player);
    game->player.position = (Vector2){3 * TILE_SIZE, 3 * TILE_SIZE};
    AcquireWeapon(&game->player, SWORD);

    //TODO: testing only, remove once levels place their own enemies
    SpawnRandomEnemies(&game->worlds[0], RED_SLIME, 10);
    SpawnRandomEnemies(&game->worlds[0], WIZ, 5);

    game->camera = (Camera2D){
        .offset = {floorf(GetScreenWidth() / 2.0f), floorf(GetScreenHeight() / 2.0f)},
        .target = game->player.position,
        .rotation = 0.0f,
        .zoom = 6.0f,
    };
}

void DestroyGame(Game* game)
{
    for (int i = 0; i < MAX_WORLDS; i++)
    {
        free(game->worlds[i].tile_map.tiles);
    }
}

static World* GetCurrentWorld(Game* game)
{
    return &game->worlds[game->current_world_idx];
}

static void ChangeState(Game* game, const GameState state)
{
    game->state = state;
    game->menu_selection = 0;
}

static void UpdateTitleState(Game* game)
{
    switch (UpdateMenu(&TITLE_MENU, &game->menu_selection))
    {
    case TITLE_MENU_START: ChangeState(game, GAME_STATE_PLAYING);
        break;
    case TITLE_MENU_QUIT: game->quit = true;
        break;
    default: break;
    }
}

static void UpdatePlayingState(Game* game)
{
    if (IsKeyPressed(KEY_ESCAPE)) ChangeState(game, GAME_STATE_MENU);
    if (IsKeyPressed(KEY_P)) ChangeState(game, GAME_STATE_PAUSED);
    if (IsKeyPressed(KEY_F1)) ChangeState(game, GAME_STATE_EDITOR);

    World* world = GetCurrentWorld(game);
    MovePlayer(&game->player, &world->tile_map);
    UpdatePlayer(&game->player, world);
    UpdateWorld(world, &game->player);
}

static void UpdateEditorState(Game* game)
{
    if (IsKeyPressed(KEY_F1)) ChangeState(game, GAME_STATE_PLAYING);

    World* world = GetCurrentWorld(game);
    MovePlayer(&game->player, &world->tile_map);
    UpdateEditor(&world->tile_map, game->camera);

    if (IsKeyPressed(KEY_J)) SaveTileMap(&world->tile_map, DEFAULT_MAP_PATH);
    else if (IsKeyPressed(KEY_K)) LoadTileMap(&world->tile_map, DEFAULT_MAP_PATH);

    UpdatePlayer(&game->player, world);
}

static void UpdatePausedState(Game* game)
{
    if (IsKeyPressed(KEY_P)) ChangeState(game, GAME_STATE_PLAYING);
}

static void UpdateMenuState(Game* game)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        ChangeState(game, GAME_STATE_PLAYING);
        return;
    }

    switch (UpdateMenu(&GAME_MENU, &game->menu_selection))
    {
    case GAME_MENU_RESUME: ChangeState(game, GAME_STATE_PLAYING);
        break;
    case GAME_MENU_TITLE: //Start over with a fresh game
        DestroyGame(game);
        CreateGame(game);
        break;
    case GAME_MENU_QUIT: game->quit = true;
        break;
    default: break;
    }
}

void UpdateGame(Game* game)
{
    //Snap the camera to whole screen pixels, otherwise tile edges land between pixels and seams flicker
    const Vector2 target = Vector2Add(game->player.position, (Vector2){(float)TILE_SIZE / 2, 0});
    game->camera.target = (Vector2){
        floorf(target.x * game->camera.zoom) / game->camera.zoom,
        floorf(target.y * game->camera.zoom) / game->camera.zoom,
    };
    //Draw the player at the snapped camera position so both move in the same pixel steps
    game->player.draw_position = Vector2Subtract(game->camera.target, (Vector2){(float)TILE_SIZE / 2, 0});

    switch (game->state)
    {
    case GAME_STATE_TITLE: UpdateTitleState(game);
        break;
    case GAME_STATE_PLAYING: UpdatePlayingState(game);
        break;
    case GAME_STATE_EDITOR: UpdateEditorState(game);
        break;
    case GAME_STATE_PAUSED: UpdatePausedState(game);
        break;
    case GAME_STATE_MENU: UpdateMenuState(game);
        break;
    }
}

void DrawGame(Game* game)
{
    World* world = GetCurrentWorld(game);

    ClearBackground(DARKBLUE);

    BeginMode2D(game->camera);
    DrawWorld(world);
    if (game->state == GAME_STATE_EDITOR) DrawTileMapDebugGrid(&world->tile_map);
    DrawPlayer(&game->player);

    switch (game->state)
    {
    case GAME_STATE_PLAYING:
    case GAME_STATE_PAUSED: DrawEnemyHealthBars(world);
        break;
    case GAME_STATE_EDITOR: DrawEditorCursor(&world->tile_map, game->camera);
        break;
    default: break;
    }
    EndMode2D();

    BeginUiMode();
    switch (game->state)
    {
    case GAME_STATE_TITLE:
        DrawTitleScreen();
        DrawMenu(&TITLE_MENU, game->menu_selection);
        break;
    case GAME_STATE_PLAYING: DrawHud(&game->player, world);
        break;
    case GAME_STATE_EDITOR: DrawEditorUi(&world->tile_map, game->camera);
        break;
    case GAME_STATE_PAUSED:
        DrawHud(&game->player, world);
        DrawPauseScreen();
        break;
    case GAME_STATE_MENU: DrawGameMenu(&GAME_MENU, game->menu_selection, &game->player);
        break;
    }
    EndUiMode();
}
