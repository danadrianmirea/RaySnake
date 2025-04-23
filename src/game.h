#pragma once

#include "raylib.h"
#include "snake.h"
#include "food.h"

class Game
{
public:
    Game();
    void InitGame();
    ~Game();
    void Reset();

    void Update(float dt);
    void HandleInput();
    void UpdateUI();
    void Draw();
    void DrawUI();

    void CheckCollisionWithFood();
    void CheckCollisionWithEdges();
    void CheckCollisionWithSnake();
    void GameOver();
    std::string FormatWithLeadingZeroes(int number, int width);
    
    int GetScore() { return score; }

    void CheckForHighScore();
    void SaveHighScoreToFile();
    int LoadHighScoreFromFile();

    bool firstTimeGameStart;
    bool isFirstFrameAfterReset;
    bool isInExitMenu;
    bool paused;
    bool lostWindowFocus;
    bool gameOver;

private:
    Snake snake;
    Food food = Food(snake.body);
    Sound eatSound;
    Sound wallSound;

    int score;
    int highScore;
    bool won;
    
    float timePassedSinceLastSnakeUpdate;
    float snakeUpdateTime;
    const float snakeUpdateSpeedIncrement = 0.005;
    //const int maxPoints = 50;
    const float snakeUpdateTimeLimit = 0.09f;

    float screenScale;
    RenderTexture2D targetRenderTex;
    Font font;

    bool inputProcessed;
};
