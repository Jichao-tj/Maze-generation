/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*b
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "maze.h"
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <random>

const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 20;
const int mazeWidth = screenWidth / cellSize;
const int mazeHeight = screenHeight / cellSize;

struct Cell {
    int x, y;

    Cell(int x, int y) : x(x), y(y) {}
};

enum class MazeState {
    Idle,
    Generating,
    Done
};

std::vector<std::vector<bool>> maze(mazeHeight, std::vector<bool>(mazeWidth, true));

void drawMaze() {
    for (int y = 0; y < mazeHeight; ++y) {
        for (int x = 0; x < mazeWidth; ++x) {
            if (maze[y][x]) {
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, DARKGRAY);
            }
            else {
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, RAYWHITE);
            }
        }
    }
}

bool generateMazeStepByStep(int startX, int startY, MazeState& state) {
    static std::stack<Cell> stack;
    static std::random_device rd;
    static std::default_random_engine eng(rd());

    int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    switch (state) {
    case MazeState::Idle:
        // Start the maze generation
        stack.push(Cell(startX, startY));
        maze[startY][startX] = false;
        state = MazeState::Generating;
        break;

    case MazeState::Generating:
        if (!stack.empty()) {
            int x = stack.top().x;
            int y = stack.top().y;

            bool found = false;
            for (int i = 0; i < 4; ++i) {
                int nx = x + 2 * directions[i][0];
                int ny = y + 2 * directions[i][1];

                if (nx >= 0 && nx < mazeWidth && ny >= 0 && ny < mazeHeight && maze[ny][nx]) {
                    found = true;
                    break;
                }
            }

            if (found) {
                std::shuffle(std::begin(directions), std::end(directions), eng);

                for (const auto& dir : directions) {
                    int nx = x + 2 * dir[0];
                    int ny = y + 2 * dir[1];

                    if (nx >= 0 && nx < mazeWidth && ny >= 0 && ny < mazeHeight && maze[ny][nx]) {
                        maze[ny][nx] = false;
                        maze[y + dir[1]][x + dir[0]] = false;

                        drawMaze();
                        DrawRectangle(nx * cellSize, ny * cellSize, cellSize, cellSize, GREEN);
                        DrawRectangle((x + dir[0]) * cellSize, (y + dir[1]) * cellSize, cellSize, cellSize, GREEN);

                        stack.push(Cell(nx, ny));
                        return true;
                    }
                }
            }
            else {
                stack.pop();
            }
        }
        else {
            state = MazeState::Done;
            return false;
        }
        break;

    case MazeState::Done:
        return false;
    }

    return false;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{    
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "maze");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    double lastTime = GetTime();
    double delay = 0.05;
    MazeState state = MazeState::Idle;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
 
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        double currentTime = GetTime();
        if ((currentTime - lastTime) >= delay && generateMazeStepByStep(0, 0, state)) {
            // Continue generating while true and the elapsed time is greater than or equal to the delay
            lastTime = currentTime;
        }

        drawMaze();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseAudioDevice();
    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}