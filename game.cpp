#include "game.h"
#include "globals.h"

Game::Game()
{
    InitAudioDevice();
    eatSound = LoadSound("Sounds/eat.mp3");
    wallSound = LoadSound("Sounds/wall.mp3");
    Reset();
}

Game::~Game()
{
    UnloadSound(eatSound);
    UnloadSound(wallSound);
    CloseAudioDevice();
}

void Game::Reset()
{
    running = true;
    paused = false;
    score = 0;
    won = false;
    timePassedSinceLastSnakeUpdate = 0.0f;
    snakeUpdateTime = 0.15f;
    snake.Reset();
    food.position = food.GenerateRandomPos(snake.body);
}

void Game::Update()
{
    if (running)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            running = false;
            paused = true;
            return;
        }

        // check if game is won
        // if ((int)snake.body.size() > cellCount * cellCount - 1)
        if (score >= maxPoints)
        {
            won = true;
            GameOver();
            return;
        }

        // snake update logic
        timePassedSinceLastSnakeUpdate += GetFrameTime();
        if (timePassedSinceLastSnakeUpdate >= snakeUpdateTime)
        {
            timePassedSinceLastSnakeUpdate = 0.0f;
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithSnake();
            if (running)
            {
                snake.Update();
            }
        }

        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            if (!Vector2Equals(snake.direction, Vector2{0, 1}))
            {
                snake.direction = {0, -1};
            }
        }

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            if (!Vector2Equals(snake.direction, Vector2{0, -1}))
            {
                snake.direction = {0, 1};
            }
        }

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        {
            if (!Vector2Equals(snake.direction, Vector2{1, 0}))
            {
                snake.direction = {-1, 0};
            }
        }

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        {
            if (!Vector2Equals(snake.direction, Vector2{-1, 0}))
            {
                snake.direction = {1, 0};
            }
        }
    }
    else if (paused)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            paused = false;
            running = true;
            return;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_SPACE))
        {

            Reset();
        }
    }
}

void Game::CheckCollisionWithFood()
{

    if (Vector2Equals(snake.body[0], food.position))
    {
        food.position = food.GenerateRandomPos(snake.body);

        score += 2;
        snake.Grow();
        PlaySound(eatSound);
        snakeUpdateTime -= snakeUpdateSpeedIncrement; // make snake move faster everytime it eats something
    }
}

void Game::CheckCollisionWithEdges()
{
    if (snake.body[0].x == cellCount || snake.body[0].x == -1)
    {
        GameOver();
    }

    if (snake.body[0].y == cellCount || snake.body[0].y == -1)
    {
        GameOver();
    }
}

void Game::CheckCollisionWithSnake()
{
    for (int i = 1; i < (int)snake.body.size(); i++)
    {
        if (Vector2Equals(snake.body[i], snake.body[0]))
        {
            GameOver();
        }
    }
}

void Game::GameOver()
{
    running = false;
    if (!won)
    {
        PlaySound(wallSound);
    }
}

void Game::Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawUI();
    snake.Draw();
    food.Draw();
    EndDrawing();
}

void Game::DrawUI()
{
    DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5.0f, WHITE);
    DrawText(TextFormat("%d", score), offset - 5, offset + cellSize * cellCount + 10, 40, WHITE);

    DrawText(TextFormat("Reach %d points to win", maxPoints), offset - 5 + 250, offset + cellSize * cellCount + 10, 40, WHITE);

    if (running == false && paused == false)
    {
        if (won)
        {
            DrawText("You win! Press SPACE to play again", 80, 10, 40, WHITE);
        }
        else
        {
            DrawText("Game over, press SPACE to play again", 80, 10, 40, WHITE);
        }
    }
    else
    {
        if (paused)
        {
            DrawText("Paused. Press SPACE to continue", 80, 10, 40, WHITE);
        }
        else
        {
            DrawText("Press SPACE to pause", 200, 10, 40, WHITE);
        }
    }
}
