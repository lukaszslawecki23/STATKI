#pragma once
#include <SDL3/SDL.h>

enum class CellState
{
    Empty,
    Clicked
};

class Board
{
    private:
        static constexpr int gridSize = 10;
        int offset_x;
        int offset_y;
        int cell_size = 40;
        int gap = 2;
        int step = cell_size + gap;

        CellState cells[gridSize][gridSize] = {};
    public:
        Board(int x, int y);
        void handleClick(float mouseX, float mouseY);
        void render(SDL_Renderer* renderer) const;

        int getGridSize() const { return gridSize; }
};