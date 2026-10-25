#include <raylib.h>
#include <stdlib.h>

#include "../include/assets.h"
#include "../include/game.h"

#define RAYGUI_IMPLEMENTATION
#include <raymath.h>

#include "../include/raygui.h"

int main()
{
    //Innit the Window
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(0, 0, "Yet Another 2D Adventure Game");
    SetTargetFPS(GetMonitorRefreshRate(0));
    SetExitKey(KEY_NULL); //ESC is used for the menu, closing is handled by the window manager

    //Innit Audio
    InitAudioDevice();
    if (!IsAudioDeviceReady())
    {
        CloseWindow();
        return EXIT_FAILURE;
    }
    SetMasterVolume(.25f);

    //Innit Game
    LoadAssets();
    Game game;
    CreateGame(&game);

    while (!WindowShouldClose() && !game.quit)
    {
        if (!IsSoundPlaying(assets.ambient_forrest)) PlaySound(assets.ambient_forrest);
        UpdateGame(&game);

        BeginDrawing();

        DrawGame(&game);

        EndDrawing();
    }

    DestroyGame(&game);
    UnloadAssets();
    CloseWindow();
}
