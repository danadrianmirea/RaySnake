#pragma once

#include <deque>
#include "raylib.h"
#include <raymath.h>

using std::deque;

class Snake
{
public:
    // Static textures for snake segments
    static Texture2D headUp;
    static Texture2D headDown;
    static Texture2D headLeft;
    static Texture2D headRight;
    static Texture2D tailUp;
    static Texture2D tailDown;
    static Texture2D tailLeft;
    static Texture2D tailRight;
    static Texture2D bodyHorizontal;
    static Texture2D bodyVertical;
    static Texture2D bodyTopLeft;
    static Texture2D bodyTopRight;
    static Texture2D bodyBottomLeft;
    static Texture2D bodyBottomRight;

    static void InitTextures();
    static void UnloadTextures();

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
    Vector2 direction;
    bool isGrowing;
};
 