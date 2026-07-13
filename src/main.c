#include <raylib.h>
#include <stdio.h>

#include "../include/assets.h"
#include "../include/player.h"
#include "../include/tilemap.h"
#include "../include/levels.h"

int main() {

    //Innit the Window
    InitWindow(1280, 920, "test");
    SetTargetFPS(GetMonitorRefreshRate(0));

    //Load the assets
    Assets assets;
    LoadAssets(&assets);

    //create a player
    Player player;
    CreatePlayer(&player, assets.player);

    TileMap level;
    CreateLevel1(&level, assets);

    Camera2D camera = {0};
    camera.target = player.position;
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;
    camera.offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };

    float console_log_threshold = .5f;
    float console_log_update = .0f;

    while (!WindowShouldClose()) {
        camera.target = player.position;
        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(BLACK);
        DrawTileMap(&level);
        DrawPlayer(&player);

        EndMode2D();
            //update game
            MovePlayer(&player, &level);

        EndDrawing();

        console_log_update += GetFrameTime();
        if (console_log_update > console_log_threshold)
        {
            console_log_update = .0f;
            printf("Position player: {%f, %f}\n", player.position.x, player.position.y);
        }


    }

        CloseWindow();
}
