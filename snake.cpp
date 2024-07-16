#include "snake.h"
#include "globals.h"

Snake::Snake()
{
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
    lastDirection = direction;
    isGrowing = false;
}

void Snake::Draw()
{
    for (unsigned int i = 0; i < body.size(); i++)
    {
        float x = body[i].x;
        float y = body[i].y;

        Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
        DrawRectangleRounded(segment, 0.9, 6, WHITE);
    }
}

void Snake::Grow()
{
    isGrowing = true;
}

void Snake::Reset()
{
    body.clear();
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
    lastDirection = direction;
}

void Snake::Update()
{
    if (isGrowing)
    {
        body.push_front(Vector2Add(body[0], direction));
        isGrowing = false;
    }
    else
    {
        body.pop_back();
        body.push_front(Vector2Add(body[0], direction));
    }
}
