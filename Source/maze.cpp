#include "maze.h"
#include <algorithm>
#include <random>

Maze::Maze(int window_width, int window_height, int cell_size) :
    m_window_width(window_width),
    m_window_height(window_height),
    m_cell_size(cell_size),
    m_maze_width(window_width / cell_size),
    m_maze_height(window_height / cell_size),
    m_maze(m_maze_height, std::vector<bool>(m_maze_width, true)),
    m_state(mazeState::Idle)
{
}

Maze::~Maze()
{
}

bool Maze::generate_maze(int start_x, int start_y)
{
    static std::stack<Cell> stack;
    static std::random_device rd;
    static std::default_random_engine eng(rd());

    int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    switch (m_state) {
    case mazeState::Idle:
        // Start the maze generation
        stack.push(Cell(start_x, start_y));
        m_maze[start_y][start_x] = false;
        m_state = mazeState::Generating;
        break;

    case mazeState::Generating:
        if (!stack.empty()) {
            int x = stack.top().x;
            int y = stack.top().y;

            bool found = false;
            for (int i = 0; i < 4; ++i) {
                int nx = x + 2 * directions[i][0];
                int ny = y + 2 * directions[i][1];

                if (nx >= 0 && nx < m_maze_width && ny >= 0 && ny < m_maze_height && m_maze[ny][nx]) {
                    found = true;
                    break;
                }
            }

            if (found) {
                std::shuffle(std::begin(directions), std::end(directions), eng);

                for (const auto& dir : directions) {
                    int nx = x + 2 * dir[0];
                    int ny = y + 2 * dir[1];

                    if (nx >= 0 && nx < m_maze_width && ny >= 0 && ny < m_maze_height && m_maze[ny][nx]) {
                        m_maze[ny][nx] = false;
                        m_maze[y + dir[1]][x + dir[0]] = false;

                        draw();
                        DrawRectangle(nx * m_cell_size, ny * m_cell_size, m_cell_size, m_cell_size, GREEN);
                        DrawRectangle((x + dir[0]) * m_cell_size, (y + dir[1]) * m_cell_size, m_cell_size, m_cell_size, GREEN);

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
            m_state = mazeState::Done;
            return false;
        }
        break;

    case mazeState::Done:
        return false;
    }

    return false;
}

void Maze::draw()
{
    for (int y = 0; y < m_maze_height; ++y) {
        for (int x = 0; x < m_maze_width; ++x) {
            if (m_maze[y][x]) {
                DrawRectangle(x * m_cell_size, y * m_cell_size, m_cell_size, m_cell_size, DARKGRAY);
            }
            else {
                DrawRectangle(x * m_cell_size, y * m_cell_size, m_cell_size, m_cell_size, RAYWHITE);
            }
        }
    }
}