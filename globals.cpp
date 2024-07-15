#include "raylib.h"
#include "globals.h"
#include "raymath.h"

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

Color grey = Color{ 29, 29, 27, 255 };
Color yellow = Color{ 243, 216, 63, 255 };
int windowWidth = 1920;
int windowHeight = 1080;
const int gameScreenWidth = 800;
const int gameScreenHeight = 800;
bool exitWindowRequested = false;
bool exitWindow = false;
bool fullscreen = true;
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
