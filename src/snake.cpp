#include "snake.h"
#include "globals.h"

// Initialize static texture members
Texture2D Snake::headUp;
Texture2D Snake::headDown;
Texture2D Snake::headLeft;
Texture2D Snake::headRight;
Texture2D Snake::tailUp;
Texture2D Snake::tailDown;
Texture2D Snake::tailLeft;
Texture2D Snake::tailRight;
Texture2D Snake::bodyHorizontal;
Texture2D Snake::bodyVertical;
Texture2D Snake::bodyTopLeft;
Texture2D Snake::bodyTopRight;
Texture2D Snake::bodyBottomLeft;
Texture2D Snake::bodyBottomRight;

void Snake::InitTextures()
{
    // Load head textures
    Image headUpImg = LoadImage("Graphics/head_up.png");
    headUp = LoadTextureFromImage(headUpImg);
    UnloadImage(headUpImg);

    Image headDownImg = LoadImage("Graphics/head_down.png");
    headDown = LoadTextureFromImage(headDownImg);
    UnloadImage(headDownImg);

    Image headLeftImg = LoadImage("Graphics/head_left.png");
    headLeft = LoadTextureFromImage(headLeftImg);
    UnloadImage(headLeftImg);

    Image headRightImg = LoadImage("Graphics/head_right.png");
    headRight = LoadTextureFromImage(headRightImg);
    UnloadImage(headRightImg);

    // Load tail textures
    Image tailUpImg = LoadImage("Graphics/tail_up.png");
    tailUp = LoadTextureFromImage(tailUpImg);
    UnloadImage(tailUpImg);

    Image tailDownImg = LoadImage("Graphics/tail_down.png");
    tailDown = LoadTextureFromImage(tailDownImg);
    UnloadImage(tailDownImg);

    Image tailLeftImg = LoadImage("Graphics/tail_left.png");
    tailLeft = LoadTextureFromImage(tailLeftImg);
    UnloadImage(tailLeftImg);

    Image tailRightImg = LoadImage("Graphics/tail_right.png");
    tailRight = LoadTextureFromImage(tailRightImg);
    UnloadImage(tailRightImg);

    // Load body textures
    Image bodyHorizontalImg = LoadImage("Graphics/body_horizontal.png");
    bodyHorizontal = LoadTextureFromImage(bodyHorizontalImg);
    UnloadImage(bodyHorizontalImg);

    Image bodyVerticalImg = LoadImage("Graphics/body_vertical.png");
    bodyVertical = LoadTextureFromImage(bodyVerticalImg);
    UnloadImage(bodyVerticalImg);

    Image bodyTopLeftImg = LoadImage("Graphics/body_topleft.png");
    bodyTopLeft = LoadTextureFromImage(bodyTopLeftImg);
    UnloadImage(bodyTopLeftImg);

    Image bodyTopRightImg = LoadImage("Graphics/body_topright.png");
    bodyTopRight = LoadTextureFromImage(bodyTopRightImg);
    UnloadImage(bodyTopRightImg);

    Image bodyBottomLeftImg = LoadImage("Graphics/body_bottomleft.png");
    bodyBottomLeft = LoadTextureFromImage(bodyBottomLeftImg);
    UnloadImage(bodyBottomLeftImg);

    Image bodyBottomRightImg = LoadImage("Graphics/body_bottomright.png");
    bodyBottomRight = LoadTextureFromImage(bodyBottomRightImg);
    UnloadImage(bodyBottomRightImg);
}

void Snake::UnloadTextures()
{
    UnloadTexture(headUp);
    UnloadTexture(headDown);
    UnloadTexture(headLeft);
    UnloadTexture(headRight);
    UnloadTexture(tailUp);
    UnloadTexture(tailDown);
    UnloadTexture(tailLeft);
    UnloadTexture(tailRight);
    UnloadTexture(bodyHorizontal);
    UnloadTexture(bodyVertical);
    UnloadTexture(bodyTopLeft);
    UnloadTexture(bodyTopRight);
    UnloadTexture(bodyBottomLeft);
    UnloadTexture(bodyBottomRight);
}

Snake::Snake()
{
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
    isGrowing = false;
}

void Snake::Draw()
{
    for (unsigned int i = 0; i < body.size(); i++)
    {
        float x = body[i].x;
        float y = body[i].y;
        Vector2 pos = Vector2{offset + x * cellSize, offset + y * cellSize};

        if (i == 0) // Head
        {
            if (direction.x == 1) DrawTexture(headRight, pos.x, pos.y, WHITE);
            else if (direction.x == -1) DrawTexture(headLeft, pos.x, pos.y, WHITE);
            else if (direction.y == 1) DrawTexture(headDown, pos.x, pos.y, WHITE);
            else DrawTexture(headUp, pos.x, pos.y, WHITE);
        }
        else if (i == body.size() - 1) // Tail
        {
            Vector2 prevPos = body[i - 1];
            if (prevPos.x < x) DrawTexture(tailRight, pos.x, pos.y, WHITE);
            else if (prevPos.x > x) DrawTexture(tailLeft, pos.x, pos.y, WHITE);
            else if (prevPos.y < y) DrawTexture(tailDown, pos.x, pos.y, WHITE);
            else DrawTexture(tailUp, pos.x, pos.y, WHITE);
        }
        else // Body
        {
            Vector2 prevPos = body[i - 1];
            Vector2 nextPos = body[i + 1];

            Vector2 dirToPrev = Vector2Subtract(prevPos, body[i]);
            Vector2 dirToNext = Vector2Subtract(nextPos, body[i]);

            // Straight segment
            if ((dirToPrev.y == 0 && dirToNext.y == 0))
                DrawTexture(bodyHorizontal, pos.x, pos.y, WHITE);
            else if ((dirToPrev.x == 0 && dirToNext.x == 0))
                DrawTexture(bodyVertical, pos.x, pos.y, WHITE);
            else
            {
                // Corner segment
                // Up + Right
                if (( (dirToPrev.x == 1 && dirToNext.y == -1) || (dirToNext.x == 1 && dirToPrev.y == -1) ))
                    DrawTexture(bodyTopRight, pos.x, pos.y, WHITE);
                // Up + Left
                else if (( (dirToPrev.x == -1 && dirToNext.y == -1) || (dirToNext.x == -1 && dirToPrev.y == -1) ))
                    DrawTexture(bodyTopLeft, pos.x, pos.y, WHITE);
                // Down + Right
                else if (( (dirToPrev.x == 1 && dirToNext.y == 1) || (dirToNext.x == 1 && dirToPrev.y == 1) ))
                    DrawTexture(bodyBottomRight, pos.x, pos.y, WHITE);
                // Down + Left
                else if (( (dirToPrev.x == -1 && dirToNext.y == 1) || (dirToNext.x == -1 && dirToPrev.y == 1) ))
                    DrawTexture(bodyBottomLeft, pos.x, pos.y, WHITE);
            }
        }
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
