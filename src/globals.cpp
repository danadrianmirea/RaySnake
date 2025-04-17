#include "raylib.h"
#include "globals.h"
#include "raymath.h"

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

const int cellSize = 30;
const int cellCount = 25;
const int offset = 75;

Color grey = Color{ 29, 29, 27, 255 };
Color yellow = Color{ 243, 216, 63, 255 };
int windowWidth = 1920;
int windowHeight = 1080;
const int gameScreenWidth = 2 * offset + cellSize * cellCount;
const int gameScreenHeight = 2 * offset + cellSize * cellCount;

bool exitWindowRequested = false;
bool exitWindow = false;
#ifdef EMSCRIPTEN_BUILD
bool fullscreen = false;
#else
bool fullscreen = true;
#endif
const int minimizeOffset = 50;
float borderOffsetWidth = 20.0;
float borderOffsetHeight = 50.0f;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}
