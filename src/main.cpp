#include <iostream>
#include <raylib.h>
#include "globals.h"
#include "game.h"

#ifdef EMSCRIPTEN_BUILD
#include <emscripten.h>

Game* game = nullptr;

void mainLoop() {
    game->Update(GetFrameTime());
    game->Draw();
}
#endif

int main()
{
    InitWindow(gameScreenWidth, gameScreenHeight, "Snake");
    SetMasterVolume(0.22f);
    SetExitKey(KEY_NULL);
    SetWindowPosition(50, 50);
    SetTargetFPS(144);

    Game gameInstance;
    #ifdef EMSCRIPTEN_BUILD
    game = &gameInstance;
    emscripten_set_main_loop(mainLoop, 0, 1);
    #else
    ToggleBorderlessWindowed();
    float dt = 0.0f;

    while (!exitWindow)
    {
        gameInstance.Update(dt);
        gameInstance.Draw();
    }
    #endif

    CloseWindow();
    return 0;
}