#pragma once

#include <deque>
#include "raylib.h"
#include <raymath.h>

using std::deque;

class Snake
{
public:
    Snake();
    Snake(const Snake &) = delete;
    Snake &operator=(const Snake &) = delete;
    Snake(Snake &&) = delete;
    Snake &operator=(Snake &&) = delete;

    void Draw();
    void Grow();
    void Reset();
    void Update();

    // deque<Vector2> body = {Vector2{0, 0}};
    deque<Vector2> body;
    Vector2 lastDirection;
    Vector2 direction;
    bool isGrowing;
};
 