#include "food.h"
#include "globals.h"

Texture2D Food::texture;
bool Food::isTextureLoaded = false;

void Food::InitTexture()
{
    if (isTextureLoaded)
    {
        return;
    }
    Image image = LoadImage("Graphics/apple.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    isTextureLoaded = true;
}

void Food::UnloadTexture()
{
    if (!isTextureLoaded)
    {
        return;
    }
    ::UnloadTexture(texture);
    isTextureLoaded = false;
}

Food::Food(deque<Vector2> snakeBody)
{
    position = GenerateRandomPos(snakeBody);
}

Food::~Food()
{
}

void Food::Draw()
{
    DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
}

Vector2 Food::GenerateRandomCell()
{
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    return Vector2{x, y};
}

Vector2 Food::GenerateRandomPos(deque<Vector2> snakeBody)
{
    Vector2 position = GenerateRandomCell();

    while (ElementInDeque(position, snakeBody))
    {
        position = GenerateRandomCell();
    }

    return position;
}
