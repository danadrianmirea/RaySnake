cmake_minimum_required(VERSION 3.10)
project(RaySnake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add definition to fix MinGW-w64 quick_exit issues
add_definitions(-D_GLIBCXX_USE_C99_STDLIB)

# Add static linking flags for MSYS/GCC
if(MSYS OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

# Set Windows subsystem for release builds
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS")
elseif(MSYS OR MINGW)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mwindows")
endif()

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

# Copy assets to output directory and create a working directory
set(ASSETS_DIR "${CMAKE_BINARY_DIR}/assets")
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${ASSETS_DIR})

# Set the working directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    VS_DEBUGGER_ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}"
) 