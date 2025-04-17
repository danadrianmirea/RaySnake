#pragma once

#include <deque>
#include "raylib.h"

using std::deque;

class Food
{
public:
    Vector2 position;
    Texture2D texture;
    Food() = delete;
    Food(deque<Vector2> snakeBody);
    ~Food();
    void Draw();
    Vector2 GenerateRandomCell();
    Vector2 GenerateRandomPos(deque<Vector2> snakeBody);
};
