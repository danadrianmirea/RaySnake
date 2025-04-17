#pragma once

#include "raylib.h"

#ifdef EMSCRIPTEN_BUILD
#define WEB_SCREEN_WIDTH 960
#define WEB_SCREEN_HEIGHT 540
#endif

inline int GetScreenWidthWrapper() {
#ifdef EMSCRIPTEN_BUILD
    return WEB_SCREEN_WIDTH;
#else
    return GetScreenWidth();
#endif
}

inline int GetScreenHeightWrapper() {
#ifdef EMSCRIPTEN_BUILD
    return WEB_SCREEN_HEIGHT;
#else
    return GetScreenHeight();
#endif
} 