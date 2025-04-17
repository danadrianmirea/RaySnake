cmake_minimum_required(VERSION 3.10)
project(RaySnake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add definition to fix MinGW-w64 quick_exit issues
add_definitions(-D_GLIBCXX_USE_C99_STDLIB)

# Find GLFW package
find_package(glfw3 REQUIRED)

# Set Raylib paths
set(RAYLIB_PATH "c:/raylib/raylib")
set(RAYLIB_INCLUDE_PATH "${RAYLIB_PATH}/src")
set(RAYLIB_LIBRARY_PATH "${RAYLIB_PATH}/src")

# Add Raylib include directory
include_directories(${RAYLIB_INCLUDE_PATH})

# Add source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/snake.cpp
    src/globals.cpp
    src/food.cpp
)

# Add header files
set(HEADERS
    src/game.h
    src/snake.h
    src/globals.h
    src/food.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link against Raylib, GLFW and its dependencies
target_link_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBRARY_PATH})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    libraylib.a
    glfw
    winmm
    gdi32
    opengl32
    user32
    shell32
    kernel32
    advapi32
    ws2_32
)

# Copy assets to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
) 