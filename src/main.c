#include <raylib.h>

#include "../include/assets.h"
#include "../include/game.h"

#define RAYGUI_IMPLEMENTATION
#include <raymath.h>

#include "../include/raygui.h"

int main()
{
    //Innit the Window
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Yet Another 2D Adventure Game");
    SetTargetFPS(GetMonitorRefreshRate(0));
    SetExitKey(KEY_LEFT_ALT || KEY_F4);

    //Innit Audio
    InitAudioDevice();
    if (!IsAudioDeviceReady())
    {
        return EXIT_FAILURE;
    }
    SetMasterVolume(.25f);

    //Innit Game
    LoadAssets();
    Game game;
    CreateGame(&game);

    while (!WindowShouldClose())
    {
        if (!IsSoundPlaying(assets.ambient_forrest)) PlaySound(assets.ambient_forrest);
        UpdateGame(&game);

        BeginDrawing();

        DrawGame(&game);

        DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() / 2 - 40, 0, 28, RAYWHITE);
        DrawText(TextFormat("%d", game.state), GetScreenWidth() / 2 - 40, 30, 28, RAYWHITE);

        EndDrawing();
    }

    DestroyGame(&game);
    UnloadAssets();
    CloseWindow();
}
