#pragma once

#include <vector>

#include "raylib.h"
#include "snake.h"
#include "food.h"

class Game
{
public:
    static bool isMobile;
    Game();
    void InitGame();
    ~Game();
    void Reset();

    void Update(float dt);
    void ReadInput();
    void ProcessInput();
    void UpdateUI();
    void Draw();
    void DrawUI();

    void CheckCollisions();
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
    Music music; // Background music
    bool musicPaused = false;

    int score;
    int highScore;
    bool won;
    
    float timePassedSinceLastSnakeUpdate;
    float snakeUpdateTime;
    const float snakeUpdateSpeedIncrement = 0.005;
    const float snakeUpdateTimeLimit = 0.09f;

    float screenScale;
    RenderTexture2D targetRenderTex;
    Font font;

    // keyboard state tracking
    enum KeyPress {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    std::vector<bool> keyPresses;

    // Touch state tracking
    Vector2 touchStartPos;
    Vector2 touchMovement;
    const float SWIPE_THRESHOLD = 20.0f; // Minimum distance for a swipe
};
