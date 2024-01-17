#pragma once

#include "raylib.h"
#include <vector>
#include <stack>

class Maze
{
private:
    struct Cell {
        int x, y;

        Cell(int x, int y) : x(x), y(y) {}
    };

    enum class mazeState {
        Idle,
        Generating,
        Done
    };
    int m_window_width = 800;
    int m_window_height = 600;
    int m_cell_size = 20;
    int m_maze_width = m_window_width / m_cell_size;
    int m_maze_height = m_window_height / m_cell_size;
    std::vector<std::vector<bool>> m_maze;
    mazeState m_state;
public:
    Maze(int window_width, int window_height, int cell_size);
    ~Maze();
    bool generate_maze(int start_x, int start_y);
    void draw();
};