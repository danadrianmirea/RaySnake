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
    void GameWin();
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
    bool won;  // New variable to track win state

private:
    Snake snake;
    Food food = Food(snake.body);
    Sound eatSound;
    Sound wallSound;
    Music music; // Background music
    bool musicPaused = false;

    int score;
    int highScore;

    static constexpr float startingSnakeUpdateTime = 0.1f;
    static constexpr float snakeUpdateTimeIncrement = 0.000083333f;
    static constexpr float snakeUpdateTimeLimit = 0.05f;
    
    float timePassedSinceLastSnakeUpdate;
    float snakeUpdateTime;

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
