#include <iostream>
#include <raylib.h>
#include "globals.h"
#include "game.h"

int main()
{
    InitWindow(gameScreenWidth, gameScreenHeight, "Snake");
    SetMasterVolume(0.22f);
    SetExitKey(KEY_NULL);
    SetWindowPosition(50, 50);
    SetTargetFPS(144);

    Game game;
    ToggleBorderlessWindowed();
    float dt = 0.0f;

    while (!exitWindow)
    {
        game.Update(dt);
        game.Draw();
    }

    CloseWindow();
    return 0;
}