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

    //Load the assets
    Assets assets;
    LoadAssets(&assets);

    //create a spritesheet_player
    Player player;
    CreatePlayer(&player, assets.spritesheet_player);
    player.position = (Vector2){40.f, 20.f};
    AcquireWeapon(&player, SWORD);

    TileMap level;
    CreateLevel1(&level, assets);

    World world;
    CreateWorld(&world, 0, level);

    Enemy enemy;
    CreateRedSlime(&enemy, assets.spritesheet_red_slime);
    enemy.position = (Vector2){20, 20};
    AddEnemyToWorld(&world, enemy);
    CreateWiz(&enemy, assets.spritesheet_dark_wiz);
    enemy.position = (Vector2) {100 ,200};
    AddEnemyToWorld(&world, enemy);



    Camera2D camera = {0};
    camera.target = player.position;
    camera.rotation = 0.0f;
    camera.zoom = 6.0f;
    camera.offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };

    while (!WindowShouldClose())
    {
        camera.target = player.position;
        MovePlayer(&player, &world.map);
        UpdateWorld(&world, &player);
        UpdatePlayer(&player, &world);
        //UpdateEditor(&world.map, camera);

        BeginDrawing();

        BeginMode2D(camera);
        ClearBackground(BLACK);
        DrawWorld(&world);
        DrawPlayer(&player);
        EndMode2D();

        DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() / 2 - 40, 0, 20, WHITE);

        EndDrawing();
    }

    UnloadAssets(&assets);
    CloseWindow();
}
