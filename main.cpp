#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

const float snakeUpdateTime = 0.2f;
float timePassedSinceLastSnakeUpdate = 0.0f;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

class Snake
{
public:
    //deque<Vector2> body = {Vector2{0, 0}};
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool isGrowing = false;

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;

            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.9, 6, darkGreen);
        }
    }

    void Grow()
    {
        isGrowing = true;
    }

    void Reset()
    {
        body.clear();
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }

    void Update()
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
};

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        Vector2 position = GenerateRandomCell();

        while (ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }

        return position;
    }
};

class Game
{
public:
    Snake snake;
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    bool won = false;

    Sound eatSound;
    Sound wallSound;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Update()
    {
        if (running)
        {
            // check if game is won
            if(snake.body.size() > cellCount*cellCount-1)
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
                if(running)
                {
                    snake.Update();
                }
            }

            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            {
                if( !Vector2Equals(snake.direction, Vector2{0, 1}) )
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
        else
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                running = true;
                snake.Reset();
                food.position = food.GenerateRandomPos(snake.body);
                score = 0;
                won = false;
            }
        }
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.Grow();
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges()
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

    void CheckCollisionWithSnake()
    {
        for (int i = 1; i < snake.body.size(); i++)
        {
            if (Vector2Equals(snake.body[i], snake.body[0]))
            {
                GameOver();
            }
        }
    }

    void GameOver()
    {
        running = false;
        if(!won)
        {
            PlaySound(wallSound);
        }
    }

    void Draw()
    {
        if(running == false)
        {
            if(won)
            {
                DrawText("You win! Press SPACE to play again", 100, 10, 40, BLACK);
            }
            else
            {
                DrawText("Game over, press SPACE to play again", 100, 10, 40, BLACK);
            }
        }
        snake.Draw();
        food.Draw();
    }
};

int main()
{
    InitWindow(2*offset + cellSize*cellCount, 2*offset + cellSize*cellCount, "Snake");
    SetTargetFPS(144);

    Game game;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        game.Update();

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float) cellSize*cellCount +10 }, 5.0f, BLACK);

        DrawText(TextFormat("%d", game.score), offset-5, offset+cellSize*cellCount+10, 40, darkGreen);

        game.Draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}