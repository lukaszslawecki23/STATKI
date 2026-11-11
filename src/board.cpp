#include "board.hpp"
#include <random>

Board::Board(int x, int y, bool isEnemy) : offset_x(x), offset_y(y), enemy(isEnemy) {};

void Board::handleClick(float mouseX, float mouseY)
{
    if (mouseX >= offset_x && mouseY >= offset_y)
    {
        int col = static_cast<int>(mouseX - offset_x) / step;
        int row = static_cast<int>(mouseY - offset_y) / step;

        if (col >= 0 && col < gridSize && row >= 0 && row < gridSize && cells[row][col] == CellState::Ship)
        {
            cells[row][col] = CellState::Hit;
        }
        else if (col >= 0 && col < gridSize && row >= 0 && row < gridSize && cells[row][col] == CellState::Empty)
        {
            cells[row][col] = CellState::Miss;
        }
    }
}

void Board::clear()
{
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            cells[i][j] = CellState::Empty;
        }
    }

    fleet.clear();
}

bool Board::canPlaceShip(int row, int col, int lenght, bool horizontal) const
{
    if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) return false;

    if (horizontal)
    {
        if (col + lenght > gridSize) return false;
    }
    else
    {
        if (row + lenght > gridSize) return false;
    }

    for (int i = 0; i < lenght; i++)
    {
        int current_row = row + (horizontal ? 0 : i);
        int current_col = col + (horizontal ? i : 0);

        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                int neighbour_r = current_row + dr;
                int neighbour_c = current_col + dc;

                if (neighbour_r >= 0 && neighbour_r < gridSize && neighbour_c >= 0 && neighbour_c < gridSize)
                {
                    if (cells[neighbour_r][neighbour_c] == CellState::Ship)
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool Board::placeShip(int row, int col, int lenght, bool horizontal)
{
    if (canPlaceShip(row, col, lenght, horizontal))
    {
        if (horizontal)
        {
            for (int i = 0; i < lenght; i++)
            {
                cells[row][col+i] = CellState::Ship;
            }
        }
        else
        {
            for (int i = 0; i < lenght; i++)
            {
                cells[row+i][col] = CellState::Ship;
            }
        }

        fleet.push_back({lenght, 0, row, col, horizontal});

        return true;
    }
    else
    {
        return false;
    }
}

void Board::randomShips()
{
    clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distCoord(0, gridSize - 1);
    std::uniform_int_distribution<int> distDir(0, 1);

    for (int lenght : unplacedShips) 
    {
        bool placed = false;
        
        while (!placed) 
        {
            int row = distCoord(gen);
            int col = distCoord(gen);
            bool horizontal = (distDir(gen) == 1);

            placed = placeShip(row, col, lenght, horizontal);
        }
    }
}

void Board::render(SDL_Renderer* renderer, bool hideShips) const
{
    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            CellState cellState = cells[row][col];

            if (hideShips && cellState == CellState::Ship)
            {
                cellState = CellState::Empty;
            }

            if (cellState == CellState::Empty)
            {
                SDL_SetRenderDrawColor(renderer, 40, 110, 180, SDL_ALPHA_OPAQUE);
            }
            else if (cellState == CellState::Ship)
            {
                SDL_SetRenderDrawColor(renderer, 220, 160, 60, SDL_ALPHA_OPAQUE);
            }
            else if (cellState == CellState::Miss)
            {
                SDL_SetRenderDrawColor(renderer, 220, 60, 60, SDL_ALPHA_OPAQUE);
            }
            else if (cellState == CellState::Hit)
            {
                SDL_SetRenderDrawColor(renderer, 20, 160, 60, SDL_ALPHA_OPAQUE);
            }

            SDL_FRect cellOne = {
                static_cast<float>(offset_x + col * step),
                static_cast<float>(offset_y + row * step),
                static_cast<float>(cell_size),
                static_cast<float>(cell_size)
            };

            SDL_RenderFillRect(renderer, &cellOne);
        }
    }
}