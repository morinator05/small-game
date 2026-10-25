#include "../include/game.h"

#include <raymath.h>

#include "../include/player.h"

#include <stdlib.h>
#include <string.h>

#include "../include/assets.h"
#include "../include/levels.h"

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
        if (IsKeyPressed(KEY_ENTER)) game->state = PLAYING;

        break;
    case PLAYING:
        if (IsKeyPressed(KEY_ESCAPE)) game->state = IN_MENU;
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
        if (IsKeyPressed(KEY_ESCAPE)) game->state = PLAYING;
        break;
    }
}

void DrawGame(Game* game)
{
    BeginMode2D(game->camera);
    ClearBackground(DARKBLUE);
    switch (game->state)
    {
    case TITLE:
        EndMode2D();

        DrawText("TITLE!", GetScreenWidth() / 2, GetScreenHeight() / 2, 50, RAYWHITE);
        break;
    case PLAYING:
        DrawWorld(&game->worlds[game->current_world_idx]);
        DrawPlayer(&game->player);
        EndMode2D();
        break;
    case LEVEL_EDITOR:
        DrawWorld(&game->worlds[game->current_world_idx]);
        DrawTileMapDebugGrid(&game->worlds[game->current_world_idx].tile_map);
        DrawPlayer(&game->player);
        EndMode2D();
        break;
    case PAUSED:
        DrawWorld(&game->worlds[game->current_world_idx]);
        DrawPlayer(&game->player);
        EndMode2D();

        DrawText("Paused!", GetScreenWidth() / 2, GetScreenHeight() / 2, 50, RAYWHITE);
        break;
    case IN_MENU:
        DrawWorld(&game->worlds[game->current_world_idx]);
        DrawPlayer(&game->player);
        EndMode2D();

        DrawText("Draw menue here", GetScreenWidth() / 2, GetScreenHeight() / 2, 50, RAYWHITE);
        break;
    }
}
