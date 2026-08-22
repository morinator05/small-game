#include <raylib.h>

#include "../include/assets.h"
#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/tilemap.h"
#include "../include/levels.h"
#include "../include/world.h"

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

int main()
{
    //Innit the Window
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Yet Another 2D Adventure Game");
    SetTargetFPS(GetMonitorRefreshRate(0));

    InitAudioDevice();
    if (!IsAudioDeviceReady())
    {
        return EXIT_FAILURE;
    }

    LoadAssets();

    //create a spritesheet_player
    Player player;
    CreatePlayer(&player);
    player.position = (Vector2){40.f, 20.f};
    AcquireWeapon(&player, UNARMED);

    TileMap level;
    CreateLevel1(&level);

    World world;
    CreateWorld(&world, 0, level);

    Enemy enemy;
    CreateRedSlime(&enemy);
    enemy.position = (Vector2){20, 20};
    AddEnemyToWorld(&world, enemy);
    CreateWiz(&enemy);
    enemy.position = (Vector2){100, 200};
    AddEnemyToWorld(&world, enemy);

    Camera2D camera = {0};
    camera.target = player.position;
    camera.rotation = 0.0f;
    camera.zoom = 7.0f;
    camera.offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };

    while (!WindowShouldClose())
    {
        if (!IsSoundPlaying(assets.ambient_forrest)) PlaySound(assets.ambient_forrest);

        camera.target = player.position;
        MovePlayer(&player, &world.map);
        UpdateWorld(&world, &player);
        UpdatePlayer(&player, &world);
        UpdateEditor(&world.map, camera);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        DrawWorld(&world);
        //DrawTileMapDebugGrid(&world.map);
        DrawPlayer(&player);
        EndMode2D();

        DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() / 2 - 40, 0, 20, WHITE);
        DrawText(TextFormat("Sound: %f", GetMasterVolume()), 0, 0, 20, WHITE);
        DrawText(TextFormat("(%d, %d)", (int)(TILE_SIZE / 2 + player.position.x) / TILE_SIZE,
                            (int)player.position.y / TILE_SIZE + 1), 0, 20, 20, WHITE);
        EndDrawing();
    }

    UnloadAssets();
    CloseWindow();
}
