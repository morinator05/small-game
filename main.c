#include <raylib.h>

#include "player.h"

int main() {
    InitWindow(800, 800, "test");
    SetTargetFPS(60);

    Player player = {
        .sprite.texture = LoadTexture("asset/test.png"),
        .sprite.source = {0, 0, 16, 16},
        .sprite.destination = {400, 400, 100, 100},
        .sprite.origin = {0, 0},
        .sprite.rotation = 0,
        .velocity = .5f,
    };

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
