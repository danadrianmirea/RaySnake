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

void Snake::InitTextures() {
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

void Snake::UnloadTextures() {
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

Snake::Snake() {
  body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
  direction = {1, 0};
  isGrowing = false;
}

void Snake::Draw() {
  for (unsigned int i = 0; i < body.size(); i++) {
    float x = body[i].x;
    float y = body[i].y;
    Vector2 pos = Vector2{offset + x * cellSize, offset + y * cellSize};

    auto drawSegment = [&](Texture2D tex) {
      DrawTexturePro(
        tex,
        Rectangle{0, 0, (float)tex.width, (float)tex.height},
        Rectangle{pos.x, pos.y, (float)cellSize, (float)cellSize},
        Vector2{0, 0},
        0.0f,
        WHITE
      );
    };

    if (i == 0) // Head
    {
      if (direction.x == 1)
        drawSegment(headRight);
      else if (direction.x == -1)
        drawSegment(headLeft);
      else if (direction.y == 1)
        drawSegment(headDown);
      else
        drawSegment(headUp);
    } else if (i == body.size() - 1) // Tail
    {
      Vector2 prevPos = body[i - 1];
      if (prevPos.x < x)
        drawSegment(tailRight);
      else if (prevPos.x > x)
        drawSegment(tailLeft);
      else if (prevPos.y < y)
        drawSegment(tailDown);
      else
        drawSegment(tailUp);
    } else // Body
    {
      Vector2 prevPos = body[i - 1];
      Vector2 nextPos = body[i + 1];

      Vector2 dirToPrev = Vector2Subtract(prevPos, body[i]);
      Vector2 dirToNext = Vector2Subtract(nextPos, body[i]);

      // Straight segment
      if ((dirToPrev.y == 0 && dirToNext.y == 0))
        drawSegment(bodyHorizontal);
      else if ((dirToPrev.x == 0 && dirToNext.x == 0))
        drawSegment(bodyVertical);
      else {
        // Corner segment
        // Up + Right
        if (((dirToPrev.x == 1 && dirToNext.y == -1) ||
             (dirToNext.x == 1 && dirToPrev.y == -1)))
          drawSegment(bodyTopRight);
        // Up + Left
        else if (((dirToPrev.x == -1 && dirToNext.y == -1) ||
                  (dirToNext.x == -1 && dirToPrev.y == -1)))
          drawSegment(bodyTopLeft);
        // Down + Right
        else if (((dirToPrev.x == 1 && dirToNext.y == 1) ||
                  (dirToNext.x == 1 && dirToPrev.y == 1)))
          drawSegment(bodyBottomRight);
        // Down + Left
        else if (((dirToPrev.x == -1 && dirToNext.y == 1) ||
                  (dirToNext.x == -1 && dirToPrev.y == 1)))
          drawSegment(bodyBottomLeft);
      }
    }
  }
}

void Snake::Grow() { isGrowing = true; }

void Snake::Reset() {
  body.clear();

#ifdef DEBUG_WITH_FULL_SNAKE
  // Fill all rows in zigzag, last row leaves last two cells empty
  for (int y = 0; y < cellCount; y++) {
    if (y % 2 == 0) {
      int xStart = 0, xEnd = cellCount, xStep = 1;
      if (y == cellCount - 1)
        xEnd = cellCount - 2;
      for (int x = xStart; x < xEnd; x += xStep) {
        body.push_front(Vector2{(float)x, (float)y});
      }
    } else {
      int xStart = cellCount - 1, xEnd = -1, xStep = -1;
      for (int x = xStart; x > xEnd; x += xStep) {
        body.push_front(Vector2{(float)x, (float)y});
      }
    }
  }
  // Add head to the front of the deque
  body.push_front(Vector2{(float)cellCount - 2, (float)cellCount - 1});
  direction = {1, 0};
#else
  body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
  direction = {1, 0};
#endif
}

void Snake::Update() {
  if (isGrowing) {
    body.push_front(Vector2Add(body[0], direction));
    isGrowing = false;
  } else {
    body.pop_back();
    body.push_front(Vector2Add(body[0], direction));
  }
}
