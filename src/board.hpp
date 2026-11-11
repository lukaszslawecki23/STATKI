#pragma once
#include <SDL3/SDL.h>
#include <vector>

enum class CellState
{
    Empty,
    Ship,
    Hit,
    Miss
};

struct Ship
{
    int lenght;
    int hits = 0;

    int startRow;
    int startCol;
    bool horizontal;

    bool isSunk() const
    {
        if (hits == lenght)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class Board
{
    private:
        static constexpr int gridSize = 10;
        int offset_x;
        int offset_y;
        int cell_size = 50;
        int gap = 2;
        int step = cell_size + gap;
        bool enemy;

        CellState cells[gridSize][gridSize] = {};

        std::vector<Ship> fleet;
        std::vector<int> unplacedShips = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

    public:
        Board(int x, int y, bool isEnemy);
        void handleClick(float mouseX, float mouseY);
        void clear();
        bool canPlaceShip(int row, int col, int lenght, bool horizontal) const;
        bool placeShip(int row, int col, int lenght, bool horizontal);
        void setShip(int row, int col) { cells[row][col] = CellState::Ship; }
        void randomShips();
        void render(SDL_Renderer* renderer, bool hideShips = false) const;

        int getGridSize() const { return gridSize; }
};