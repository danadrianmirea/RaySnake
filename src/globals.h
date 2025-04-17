#pragma once

#include <deque>
using std::deque;

#include <raylib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define WEB_SCREEN_WIDTH 960
#define WEB_SCREEN_HEIGHT 540
#define WEB_SCREEN_SCALE 0.9f

//#define AM_RAY_DEBUG

extern Color green;
extern Color darkGreen;

extern const int cellSize; 
extern const int cellCount;
extern const int offset;

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
extern const int minimizeOffset;
extern float borderOffsetWidth;
extern float borderOffsetHeight;