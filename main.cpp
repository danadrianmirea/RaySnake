#include <iostream>
#include <raylib.h>
#include "globals.h"
#include "game.h"

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
    InitWindow(2*offset + cellSize*cellCount, 2*offset + cellSize*cellCount, "Snake");
    SetTargetFPS(144);

    Game game;

    while (WindowShouldClose() == false)
    {    
        game.Update();
        BeginDrawing();
        game.Draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}