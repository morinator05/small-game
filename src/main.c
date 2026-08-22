#include <raylib.h>

#include "../include/assets.h"
#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/tilemap.h"
#include "../include/levels.h"
#include "../include/world.h"

#define RAYGUI_IMPLEMENTATION
#include <raymath.h>

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
    AcquireWeapon(&player, SWORD);

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

    bool editor_mode = false;

    while (!WindowShouldClose())
    {
        if (!IsSoundPlaying(assets.ambient_forrest)) PlaySound(assets.ambient_forrest);

        camera.target = Vector2Add(player.position, (Vector2){TILE_SIZE / 2, 0});
        MovePlayer(&player, &world.map);
        UpdateWorld(&world, &player);
        UpdatePlayer(&player, &world);

        if (IsKeyPressed(KEY_F1)) editor_mode = !editor_mode;
        if (editor_mode) UpdateEditor(&world.map, camera);


        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        DrawWorld(&world);
        if (editor_mode) DrawTileMapDebugGrid(&world.map);
        DrawPlayer(&player);
        EndMode2D();

        DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() / 2 - 40, 0, 28, RAYWHITE);

        DrawText(
            TextFormat("(%d, %.2f)", player.hp, player.weapon.cooldown),
            GetWorldToScreen2D(player.position, camera).x,
            GetWorldToScreen2D(player.position, camera).y,
            38,
            RAYWHITE
        );
        for (int i = 0; i < world.enemy_count; i++)
        {
            DrawText(
            TextFormat("(%d, %.2f)", world.enemies[i].hp, world.enemies[i].hit_cooldown),
            GetWorldToScreen2D(world.enemies[i].position, camera).x,
            GetWorldToScreen2D(world.enemies[i].position, camera).y,
            38,
            RAYWHITE
        );
        }

        EndDrawing();
    }

    UnloadAssets();
    CloseWindow();
}
