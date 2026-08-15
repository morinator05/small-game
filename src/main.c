#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "../include/assets.h"
#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/tilemap.h"
#include "../include/levels.h"

int main()
{
    //Innit the Window
    InitWindow(1280, 920, "test");
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
        MovePlayer(&player, &level);
        UpdateRedSlime(&enemy, &level, &player);

        BeginDrawing();

        UpdateRedSlime(&enemy, &level, &player);
        BeginMode2D(camera);
        ClearBackground(BLACK);
        DrawTileMap(&level);
        DrawPlayer(&player);
        DrawEnemy(&enemy);
        EndMode2D();

        DrawText(TextFormat("FPS: %d", GetFPS()), 0, 0, 18, WHITE);
        DrawText(TextFormat("Position of player: {x:%3f, y:%3f}", player.position.x, player.position.y), 0,20,18,WHITE);
        DrawText(TextFormat("Distance: %3f}", Vector2Distance(player.position, enemy.position)), 0,40,18,WHITE);

        EndDrawing();
    }

    UnloadAssets(&assets);
    CloseWindow();
}
