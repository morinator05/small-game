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
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0) , "Yet Another 2D Adventure Game");
    SetTargetFPS(GetMonitorRefreshRate(0));

    //Load the assets
    Assets assets;
    LoadAssets(&assets);

    //create a player
    Player player;
    CreatePlayer(&player, assets.player);
    player.position = (Vector2){40.f, 20.f};

    TileMap level;
    CreateLevel1(&level, assets);

    Enemy enemy;
    CreateRedSlime(&enemy, assets.redSlime);
    enemy.position = (Vector2) {20,20};

    World world;
    CreateWorld(&world, 0, level);
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
        UpdateEditor(&world.map, camera);

        BeginDrawing();

        BeginMode2D(camera);
        ClearBackground(BLACK);
        DrawWorld(&world);
        DrawPlayer(&player);
        EndMode2D();

        DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() / 2 - 40, 0, 20, WHITE);
        GuiProgressBar((Rectangle) {0,0,150, 25}, "0", "max", &player.hp , 0, 1000);

        EndDrawing();
    }

    UnloadAssets(&assets);
    CloseWindow();
}
