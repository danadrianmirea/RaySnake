#include <string>
#include "game.h"
#include "globals.h"

#include <fstream>
#include <iostream>

Game::Game()
{
    firstTimeGameStart = true;

    targetRenderTex = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(targetRenderTex.texture, TEXTURE_FILTER_BILINEAR); // Texture scale filter to use

    font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    InitAudioDevice();
    eatSound = LoadSound("Sounds/eat.mp3");
    wallSound = LoadSound("Sounds/wall.mp3");

    InitGame();
}

void Game::InitGame()
{
    score = 0;
    highScore = LoadHighScoreFromFile();
    isFirstFrameAfterReset = true;
    isInExitMenu = false;
    paused = false;
    lostWindowFocus = false;
    gameOver = false;
    won = false;

    screenScale = MIN((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);

    timePassedSinceLastSnakeUpdate = 0.0f;
    snakeUpdateTime = 0.15f;
    snake.Reset();
    food.position = food.GenerateRandomPos(snake.body);
    inputProcessed = false;
}

Game::~Game()
{
    UnloadSound(eatSound);
    UnloadSound(wallSound);
    UnloadRenderTexture(targetRenderTex);
    UnloadFont(font);
    CloseAudioDevice();
}

void Game::Reset()
{
    InitGame();
}

void Game::Update(float dt)
{
    if ((int)snake.body.size() > cellCount * cellCount - 1)
    {
        won = true;
        GameOver();
        return;
    }

    screenScale = MIN((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);
    UpdateUI();

    bool running = (firstTimeGameStart == false && paused == false && lostWindowFocus == false && isInExitMenu == false && gameOver == false);
    if (running)
    {
        HandleInput();
        timePassedSinceLastSnakeUpdate += GetFrameTime();
        if (timePassedSinceLastSnakeUpdate >= snakeUpdateTime)
        {
            timePassedSinceLastSnakeUpdate = 0.0f;
            snake.Update();
            inputProcessed = false;
        }

        CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionWithSnake();
    }
}

void Game::UpdateUI()
{
    if (WindowShouldClose() || (IsKeyPressed(KEY_ESCAPE) && exitWindowRequested == false))
    {
        exitWindowRequested = true;
        isInExitMenu = true;
        return;
    }

#ifdef AM_RAY_DEBUG
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
    {
        if (fullscreen)
        {
            fullscreen = false;
            #ifndef EMSCRIPTEN_BUILD
            ToggleBorderlessWindowed();
            SetWindowPosition(minimizeOffset, minimizeOffset);
            #endif
        }
        else
        {
            fullscreen = true;
            #ifndef EMSCRIPTEN_BUILD
            ToggleBorderlessWindowed();
            #endif
        }
    }
#endif

    if (firstTimeGameStart && IsKeyPressed(KEY_SPACE))
    {
        firstTimeGameStart = false;
    }
    else if (gameOver && IsKeyPressed(KEY_SPACE))
    {
        Reset();
    }

    if (exitWindowRequested)
    {
        if (IsKeyPressed(KEY_Y))
        {
            exitWindow = true;
        }
        else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE))
        {
            exitWindowRequested = false;
            isInExitMenu = false;
        }
    }

    if (IsWindowFocused() == false)
    {
        lostWindowFocus = true;
    }
    else
    {
        lostWindowFocus = false;
    }

    if (exitWindowRequested == false && lostWindowFocus == false && gameOver == false && isFirstFrameAfterReset == false && IsKeyPressed(KEY_P))
    {
        if (paused)
        {
            paused = false;
        }
        else
        {
            paused = true;
        }
    }
}

void Game::HandleInput()
{
    if (isFirstFrameAfterReset)
    {
        isFirstFrameAfterReset = false;
        return;
    }

    if(inputProcessed)
    {
        return;
    }

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        if (!Vector2Equals(snake.direction, Vector2{0, 1}))
        {
            snake.direction = {0, -1};
        }
        inputProcessed = true;
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        if (!Vector2Equals(snake.direction, Vector2{0, -1}))
        {
            snake.direction = {0, 1};
        }
        inputProcessed = true;
    }
    else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        if (!Vector2Equals(snake.direction, Vector2{1, 0}))
        {
            snake.direction = {-1, 0};
        }
        inputProcessed = true;
    }
    else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        if (!Vector2Equals(snake.direction, Vector2{-1, 0}))
        {
            snake.direction = {1, 0};
        }
        inputProcessed = true;
    }
}

void Game::CheckCollisionWithFood()
{
    if (Vector2Equals(snake.body[0], food.position))
    {
        food.position = food.GenerateRandomPos(snake.body);

        score++;
        CheckForHighScore();
        snake.Grow();
        PlaySound(eatSound);

        if (score % 10 == 0)
        {
            if (snakeUpdateTime > snakeUpdateTimeLimit)
            {
                snakeUpdateTime -= snakeUpdateSpeedIncrement; // make snake move faster when it eats something
            }
        }
        // std::cout << "snakeUpdateTime: " << snakeUpdateTime << "\n";
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
    gameOver = true;
    PlaySound(wallSound);

    /*
    running = false;
    if (!won)
    {
        PlaySound(wallSound);
    }
    */
}

void Game::Draw()
{
    // render everything to a texture
    BeginTextureMode(targetRenderTex);
    ClearBackground(BLACK);

    snake.Draw();
    food.Draw();
    DrawUI();

    EndTextureMode();

    // render the scaled frame texture to the screen
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(targetRenderTex.texture, (Rectangle){0.0f, 0.0f, (float)targetRenderTex.texture.width, (float)-targetRenderTex.texture.height},
                   (Rectangle){(GetScreenWidth() - ((float)gameScreenWidth * screenScale)) * 0.5f, (GetScreenHeight() - ((float)gameScreenHeight * screenScale)) * 0.5f, (float)gameScreenWidth * screenScale, (float)gameScreenHeight * screenScale},
                   (Vector2){0, 0}, 0.0f, WHITE);

    DrawScreenSpaceUI();
    EndDrawing();
}

void Game::DrawUI()
{
    DrawText("Snake. WASD to play, ESC to exit, P to pause", offset, 0, 30, WHITE);
    // DrawText("", offset - 5, offset + cellSize * cellCount + 10, 40, WHITE);

    DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5.0f, WHITE);

    std::string scoreText = FormatWithLeadingZeroes(score, 7);
    DrawText("Score: ", offset - 5, offset + cellSize * cellCount + 10, 30, WHITE);
    DrawText(scoreText.c_str(), offset - 5 + 110, offset + cellSize * cellCount + 10, 30, WHITE);

    std::string highScoreText = FormatWithLeadingZeroes(highScore, 7);
    DrawText("High Score: ", offset - 5 + 400, offset + cellSize * cellCount + 10, 30, WHITE);
    DrawText(highScoreText.c_str(), offset - 5 + 590, offset + cellSize * cellCount + 10, 30, WHITE);

    /*
        DrawTextEx(font, "HIGH-SCORE", {570, 15}, 34, 2, yellow);
        std::string highScoreText = FormatWithLeadingZeroes(highScore, 7);
        DrawTextEx(font, highScoreText.c_str(), {570, 40}, 34, 2, yellow);
        */

    // DrawText(TextFormat("Reach %d points to win", maxPoints), offset - 5 + 250, offset + cellSize * cellCount + 10, 30, WHITE);

    /*
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
        */
}

std::string Game::FormatWithLeadingZeroes(int number, int width)
{
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    numberText = std::string(leadingZeros, '0') + numberText;
    return numberText;
}

void Game::CheckForHighScore()
{
    if (score > highScore)
    {
        highScore = score;
        SaveHighScoreToFile();
    }
}

void Game::SaveHighScoreToFile()
{
    #ifdef EMSCRIPTEN_BUILD
    // Use browser's localStorage for web builds
    EM_ASM_({
        Module['localStorage'].setItem("snake_highscore", $0);
    }, highScore);
    #else
    std::ofstream highScoreFile("highscore.txt");
    if (highScoreFile.is_open())
    {
        highScoreFile << highScore;
        highScoreFile.close();
    }
    else
    {
        std::cerr << "Failed to save highscore to file \n";
    }
    #endif
}

int Game::LoadHighScoreFromFile()
{
    int loadedHighScore = 0;
    #ifdef EMSCRIPTEN_BUILD
    // Use browser's localStorage for web builds
    loadedHighScore = EM_ASM_INT({
        var score = Module['localStorage'].getItem("snake_highscore");
        return score ? parseInt(score) : 0;
    });
    #else
    std::ifstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open())
    {
        highscoreFile >> loadedHighScore;
        highscoreFile.close();
    }
    else
    {
        std::cerr << "Failed to load highscore from file\n";
    }
    #endif
    return loadedHighScore;
}

void Game::DrawScreenSpaceUI()
{
    if (exitWindowRequested)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, GRAY);
        DrawText("Are you sure you want to exit? [Y/N]", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2, 40, WHITE);
    }
    else if (firstTimeGameStart)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, GRAY);
        DrawText("Press SPACE to play", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2, 40, WHITE);
    }
    else if (paused)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, GRAY);
        DrawText("Game paused, press P to continue", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2, 40, WHITE);
    }
    else if (lostWindowFocus)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, GRAY);
        DrawText("Game paused, focus window to continue", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2, 40, WHITE);
    }
    else if (gameOver)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, GRAY);
        DrawText("Game over, press SPACE to play again", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2, 40, WHITE);
    }
}
