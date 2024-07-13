#include <iostream>
#include <raylib.h>
#include "globals.h"
#include "game.h"

int main()
{
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake");
    SetTargetFPS(144);

    Game game;

    while (WindowShouldClose() == false)
    {
        game.Update();
        game.Draw();
    }

    CloseWindow();

    return 0;
}