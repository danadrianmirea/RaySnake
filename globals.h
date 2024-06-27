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

