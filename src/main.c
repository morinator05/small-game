#include <raylib.h>

#include "assets.h"
#include "player.h"
#include "tilemap.h"
#include "levels.h"

int main() {

    //Innit the Window
    InitWindow(512, 512, "test");
    SetTargetFPS(GetMonitorRefreshRate(0));

    //Load the assets
    Assets assets;
    LoadAssets(&assets);

    //create a player
    Player player;
    CreatePlayer(&player, assets.player);
    player.velocity = 150;

    TileMap level;
    CreateLevel1(&level, assets);

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(GRAY);
        DrawTileMap(&level);
        DrawPlayer(&player);

            //update game
            MovePlayer(&player);

        EndDrawing();
    }

    CloseWindow();
}
