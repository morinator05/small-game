#include <raylib.h>

#include "assets.h"
#include "player.h"

int main() {

    //Innit the Window
    InitWindow(800, 800, "test");
    SetTargetFPS(GetMonitorRefreshRate(0));

    //Load the assets
    Assets assets;
    LoadAssets(&assets);

    //create a player
    Player player;
    CreatePlayer(&player, assets.player);
    player.velocity = 1000;

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawPlayer(&player);

            //update game
            MovePlayer(&player);

        EndDrawing();
    }

    CloseWindow();
}
