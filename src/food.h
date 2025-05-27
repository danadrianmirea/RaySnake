#pragma once

#include <deque>
#include "raylib.h"

using std::deque;

class Food
{
public:
    Vector2 position;
    static Texture2D texture;
    static bool isTextureLoaded;
    static void InitTexture();
    static void UnloadTexture();
    Food() = delete;
    Food(deque<Vector2> snakeBody);
    ~Food();
    void Draw();
    Vector2 GenerateRandomCell();
    Vector2 GenerateRandomPos(deque<Vector2> snakeBody);
};
