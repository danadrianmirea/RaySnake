#pragma once

#include "raylib.h"
#include "snake.h"
#include "food.h"

class Game
{
public:
    Game();
    ~Game();
    void Reset();
    void Update();
    void CheckCollisionWithFood();
    void CheckCollisionWithEdges();
    void CheckCollisionWithSnake();
    void GameOver();
    void Draw();
    void DrawUI();
    int GetScore() { return score; }

private:
    Snake snake;
    Food food = Food(snake.body);
    Sound eatSound;
    Sound wallSound;

    bool running;
    bool paused;
    int score;
    bool won;
    float timePassedSinceLastSnakeUpdate;
    float snakeUpdateTime;
    const float snakeUpdateSpeedIncrement = 0.005;
    const int maxPoints = 50;
};
