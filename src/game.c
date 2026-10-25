#include "../include/game.h"

#include <raymath.h>

#include "../include/player.h"

#include <stdlib.h>
#include <string.h>

#include "../include/assets.h"
#include "../include/levels.h"
#include "../include/ui.h"

static const char* const TITLE_MENU_ITEMS[] = {"Start Game", "Quit"};
static const Menu TITLE_MENU = {
    .title = NULL,
    .items = TITLE_MENU_ITEMS,
    .count = 2,
    .center_x = .5f,
    .center_y = .62f
};

static const char* const GAME_MENU_ITEMS[] = {"Resume", "Back to Title", "Quit Game"};
static const Menu GAME_MENU = {
    .title = "Menu",
    .items = GAME_MENU_ITEMS,
    .count = 3,
    .center_x = .4f,
    .center_y = .5f
};

void CreateGame(Game* game)
{
    memset(game, 0, sizeof(*game));
    game->state = TITLE;

    TileMap default_tile_map = {0};
    LoadTileMap(&default_tile_map, "asset/maps/default_map.map");
    if (default_tile_map.tiles == NULL) CreateEmptyLevel(&default_tile_map);
    CreateWorld(&game->worlds[0], 0, default_tile_map);
    game->current_world_idx = 0;

    CreatePlayer(&game->player);
    game->player.position = (Vector2){3 * TILE_SIZE, 3 * TILE_SIZE};
    AcquireWeapon(&game->player, SWORD);

    //TODO: testing only, remove once levels place their own enemies
    SpawnRandomEnemies(&game->worlds[0], RED_SLIME, 10);
    SpawnRandomEnemies(&game->worlds[0], WIZ, 5);


    Camera2D camera = {0};
    camera.target = game->player.position;
    camera.rotation = 0.0f;
    camera.zoom = 6.0f;
    camera.offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };
    game->camera = camera;
}

void DestroyGame(Game* game)
{
    for (int i = 0; i < MAX_WORLDS; i++)
    {
        free(game->worlds[i].tile_map.tiles);
    }
}

void UpdateGame(Game* game)
{
    game->camera.target = Vector2Add(game->player.position, (Vector2){(float)TILE_SIZE / 2, 0});

    switch (game->state)
    {
    case TITLE:
        switch (UpdateMenu(&TITLE_MENU, &game->menu_selection))
        {
        case 0: game->state = PLAYING;
            break;
        case 1: game->quit = true;
            break;
        default: break;
        }
        break;
    case PLAYING:
        if (IsKeyPressed(KEY_ESCAPE))
        {
            game->state = IN_MENU;
            game->menu_selection = 0;
        }
        if (IsKeyPressed(KEY_P)) game->state = PAUSED;
        if (IsKeyPressed(KEY_F1)) game->state = LEVEL_EDITOR;

        MovePlayer(&game->player, &game->worlds[game->current_world_idx].tile_map);
        UpdatePlayer(&game->player, &game->worlds[game->current_world_idx]);
        UpdateWorld(&game->worlds[game->current_world_idx], &game->player);
        break;
    case LEVEL_EDITOR:
        if (IsKeyPressed(KEY_F1)) game->state = PLAYING;

        MovePlayer(&game->player, &game->worlds[game->current_world_idx].tile_map);
        UpdateEditor(&game->worlds[game->current_world_idx].tile_map, game->camera);

        if (IsKeyPressed(KEY_J))
        {
            SaveTileMap(&game->worlds[game->current_world_idx].tile_map, "asset/maps/default_map.map");
        }
        else if (IsKeyPressed(KEY_K))
        {
            LoadTileMap(&game->worlds[game->current_world_idx].tile_map, "asset/maps/default_map.map");
        }
        UpdatePlayer(&game->player, &game->worlds[game->current_world_idx]);
        break;
    case PAUSED:
        if (IsKeyPressed(KEY_P)) game->state = PLAYING;
        break;
    case IN_MENU:
        if (IsKeyPressed(KEY_ESCAPE))
        {
            game->state = PLAYING;
            break;
        }

        switch (UpdateMenu(&GAME_MENU, &game->menu_selection))
        {
        case 0: game->state = PLAYING;
            break;
        case 1: //Start over with a fresh game
            DestroyGame(game);
            CreateGame(game);
            break;
        case 2: game->quit = true;
            break;
        default: break;
        }
        break;
    }
}

void DrawGame(Game* game)
{
    World* world = &game->worlds[game->current_world_idx];

    BeginMode2D(game->camera);
    ClearBackground(DARKBLUE);
    switch (game->state)
    {
    case TITLE:
        DrawWorld(world);
        DrawPlayer(&game->player);
        EndMode2D();

        DrawTitleScreen();
        DrawMenu(&TITLE_MENU, game->menu_selection);
        break;
    case PLAYING:
        DrawWorld(world);
        DrawPlayer(&game->player);
        DrawEnemyHealthBars(world);
        EndMode2D();

        DrawHud(&game->player, world);
        break;
    case LEVEL_EDITOR:
        DrawWorld(world);
        DrawTileMapDebugGrid(&world->tile_map);
        DrawPlayer(&game->player);
        DrawEditorCursor(&world->tile_map, game->camera);
        EndMode2D();

        DrawEditorUi(&world->tile_map, game->camera);
        break;
    case PAUSED:
        DrawWorld(world);
        DrawPlayer(&game->player);
        DrawEnemyHealthBars(world);
        EndMode2D();

        DrawHud(&game->player, world);
        DrawPauseScreen();
        break;
    case IN_MENU:
        DrawWorld(world);
        DrawPlayer(&game->player);
        EndMode2D();

        DrawGameMenu(&GAME_MENU, game->menu_selection, &game->player);
        break;
    }
}
