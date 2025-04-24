#include <string>
#include "game.h"
#include "globals.h"

#include <fstream>
#include <iostream>

#ifdef EMSCRIPTEN_BUILD
#include <emscripten.h>
#endif

bool Game::isMobile = false;

Game::Game()
{
#ifdef EMSCRIPTEN_BUILD
    isMobile = EM_ASM_INT({
        return /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent);
    });
#endif
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
#ifdef EMSCRIPTEN_BUILD
    if (IsKeyPressed(KEY_ESCAPE))
    {
        paused = !paused;
        return;
    }
#else
    if (WindowShouldClose() || (IsKeyPressed(KEY_ESCAPE) && exitWindowRequested == false))
    {
        exitWindowRequested = true;
        isInExitMenu = true;
        return;
    }
#endif

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

    if (firstTimeGameStart)
    {
        if (isMobile && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            firstTimeGameStart = false;
            return;
        }
        else if (IsKeyPressed(KEY_ENTER))
        {
            firstTimeGameStart = false;
        }
    }
    else if (gameOver)
    {
        if (isMobile && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Reset();
            return;
        }
        else if (IsKeyPressed(KEY_ENTER))
        {
            Reset();
        }
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

    if (exitWindowRequested == false && lostWindowFocus == false && gameOver == false && isFirstFrameAfterReset == false)
    {
        if (isMobile)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                touchStartPos = GetMousePosition();
            }
            else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                Vector2 touchEndPos = GetMousePosition();
                Vector2 movement = Vector2Subtract(touchEndPos, touchStartPos);
                
                if (Vector2Length(movement) < SWIPE_THRESHOLD)
                {
                    // It's a tap, pause the game
                    paused = !paused;
                }
            }
        }
        else if (IsKeyPressed(KEY_P))
        {
            paused = !paused;
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

    if (isMobile && !firstTimeGameStart && !gameOver)
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            Vector2 touchEndPos = GetMousePosition();
            Vector2 movement = Vector2Subtract(touchEndPos, touchStartPos);
            
            if (Vector2Length(movement) >= SWIPE_THRESHOLD)
            {
                // Determine primary direction of swipe
                if (abs(movement.x) > abs(movement.y))
                {
                    // Horizontal swipe
                    if (movement.x > 0 && !Vector2Equals(snake.direction, Vector2{-1, 0}))
                    {
                        snake.direction = {1, 0}; // Right
                        inputProcessed = true;
                    }
                    else if (movement.x < 0 && !Vector2Equals(snake.direction, Vector2{1, 0}))
                    {
                        snake.direction = {-1, 0}; // Left
                        inputProcessed = true;
                    }
                }
                else
                {
                    // Vertical swipe
                    if (movement.y > 0 && !Vector2Equals(snake.direction, Vector2{0, -1}))
                    {
                        snake.direction = {0, 1}; // Down
                        inputProcessed = true;
                    }
                    else if (movement.y < 0 && !Vector2Equals(snake.direction, Vector2{0, 1}))
                    {
                        snake.direction = {0, -1}; // Up
                        inputProcessed = true;
                    }
                }
            }
        }
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


    EndDrawing();
}

void Game::DrawUI()
{
    float width = 700;
    float height = 120;
    float fontSize = 30;
    float textOffsetX = 300;

    if (isMobile) {
        DrawText("Snake. Swipe to control, tap to pause", offset, 0, 30, WHITE);
    } else {
        DrawText("Snake. WASD to play, ESC to exit, P to pause", offset, 0, 30, WHITE);
    }
    // DrawText("", offset - 5, offset + cellSize * cellCount + 10, 40, WHITE);

    DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5.0f, WHITE);

    std::string scoreText = FormatWithLeadingZeroes(score, 7);
    DrawText("Score: ", offset - 5, offset + cellSize * cellCount + 10, 30, WHITE);
    DrawText(scoreText.c_str(), offset - 5 + 110, offset + cellSize * cellCount + 10, 30, WHITE);

    std::string highScoreText = FormatWithLeadingZeroes(highScore, 7);
    DrawText("High Score: ", offset - 5 + 400, offset + cellSize * cellCount + 10, 30, WHITE);
    DrawText(highScoreText.c_str(), offset - 5 + 590, offset + cellSize * cellCount + 10, 30, WHITE);

    if (exitWindowRequested)
    {
        DrawRectangleRounded({(float)(gameScreenWidth - width) * 0.5f, (float)(gameScreenHeight - height) * 0.5f, width, height}, 0.76f, 20, GRAY);
        DrawText("Are you sure you want to exit? [Y/N]", (gameScreenWidth - textOffsetX * 2) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
    }
    else if (firstTimeGameStart)
    {
        DrawRectangleRounded({(float)(gameScreenWidth - width) * 0.5f, (float)(gameScreenHeight - height) * 0.5f, width, height}, 0.76f, 20, GRAY);
        if (isMobile) {
            DrawText("Tap to play", (gameScreenWidth - textOffsetX) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
        } else {
            DrawText("Press ENTER to play", (gameScreenWidth - textOffsetX) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
        }
    }
    else if (paused)
    {
        DrawRectangleRounded({(float)(gameScreenWidth - width) * 0.5f, (float)(gameScreenHeight - height) * 0.5f, width, height}, 0.76f, 20, GRAY);
#ifdef EMSCRIPTEN_BUILD
        if(isMobile)
        {
            DrawText("Game paused, tap to continue", (gameScreenWidth - textOffsetX * 2) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
        }
        else
        {
            DrawText("Game paused, press P or ESC to continue", (gameScreenWidth - textOffsetX * 2) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
        }
#else
        DrawText("Game paused, press P to continue", (gameScreenWidth - textOffsetX * 2) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
#endif
    }
    else if (lostWindowFocus)
    {
        DrawRectangleRounded({(float)(gameScreenWidth - width) * 0.5f, (float)(gameScreenHeight - height) * 0.5f, width, height}, 0.76f, 20, GRAY);
        DrawText("Game paused, focus window to continue", (gameScreenWidth - textOffsetX * 2) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
    }
    else if (gameOver)
    {
        DrawRectangleRounded({(float)(gameScreenWidth - width) * 0.5f, (float)(gameScreenHeight - height) * 0.5f, width, height}, 0.76f, 20, GRAY);
        if (isMobile) {
            DrawText("Game over, tap to play again", (gameScreenWidth - textOffsetX * 2) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
        } else {
            DrawText("Game over, press ENTER to play again", (gameScreenWidth - textOffsetX * 2) * 0.5f, (gameScreenHeight - fontSize) * 0.5f, fontSize, WHITE);
        }
    }
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
#ifndef EMSCRIPTEN_BUILD
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
#ifndef EMSCRIPTEN_BUILD

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