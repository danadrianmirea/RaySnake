cmake_minimum_required(VERSION 3.10)
project(RaySnake VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set raylib path
set(RAYLIB_PATH "C:/raylib/raylib" CACHE PATH "Path to raylib source directory")

# Configure static linking
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

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


# Add raylib as a subdirectory
add_subdirectory(${RAYLIB_PATH} ${CMAKE_BINARY_DIR}/raylib)

# Link against raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    ${RAYLIB_PATH}/src
)

# Set compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

# Set compiler flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    # Hide console window in Release builds
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
    target_link_options(${PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
    # Hide console window in Release builds for MinGW
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_RELEASE "-mwindows"
    )
    message(STATUS "Building statically linked executable")
endif()

# Copy assets to output directory and create a working directory
file(COPY ${CMAKE_SOURCE_DIR}/Font DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Graphics DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Sounds DESTINATION ${CMAKE_BINARY_DIR})

# Create zip file after build
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/zip_temp
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/Sounds ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/Sounds
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/Font ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/Font
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/Graphics ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/Graphics
        COMMAND powershell -Command "Compress-Archive -Path '${CMAKE_CURRENT_BINARY_DIR}/zip_temp/*' -DestinationPath '${CMAKE_CURRENT_BINARY_DIR}/RaySnake.zip' -Force"
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/zip_temp
        COMMENT "Creating RaySnake.zip"
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/zip_temp
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/Sounds ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/Sounds
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/Font ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/Font
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/Graphics ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/Graphics
        COMMAND zip -r ${CMAKE_CURRENT_BINARY_DIR}/RaySnake.zip ${CMAKE_CURRENT_BINARY_DIR}/zip_temp/*
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/zip_temp
        COMMENT "Creating RaySnake.zip"
    )
endif()