#pragma once

#include <deque>
using std::deque;

#include "raylib.h"

extern Color green;
extern Color darkGreen;

extern int cellSize; 
extern int cellCount;
extern int offset;

extern bool ElementInDeque(Vector2 element, deque<Vector2> deque);

extern Color grey;
extern Color yellow;
extern int windowWidth;
extern int windowHeight;
extern const int gameScreenWidth;
extern const int gameScreenHeight;
extern bool exitWindow;
extern bool exitWindowRequested;
extern bool fullscreen;
extern float borderOffsetWidth;
extern float borderOffsetHeight;