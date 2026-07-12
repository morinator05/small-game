#include <raylib.h>

#include "assets.h"
#include "player.h"

int main() {
    InitWindow(800, 800, "test");
    SetTargetFPS(60);

    Assets assets;
    LoadAssets(&assets);

    Player player;
    CreatePlayer(&player, assets.player);

    float delta = 0.f;
    const float update_threshold = .005f;

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawPlayer(&player);

        if (delta >= update_threshold) {
            //update game
            MovePlayer(&player);
            delta = 0.f;
        }
        delta += GetFrameTime();

        EndDrawing();
    }

    CloseWindow();
}
