#include "board.hpp"
#include <iostream>

Board::Board(int x, int y) : offset_x(x), offset_y(y) {};

void Board::handleClick(float mouseX, float mouseY)
{
    if (mouseX >= offset_x && mouseY >= offset_y)
    {
        int col = static_cast<int>(mouseX - offset_x) / step;
        int row = static_cast<int>(mouseY - offset_y) / step;

        if (col >= 0 && col < gridSize && row >= 0 && row < gridSize)
        {
            cells[row][col] = CellState::Clicked;
            printf("Kliknieto: %d, %d", row, col);
        }
    }
}

void Board::render(SDL_Renderer* renderer) const
{
    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            if (cells[row][col] == CellState::Empty)
            {
                SDL_SetRenderDrawColor(renderer, 40, 110, 180, SDL_ALPHA_OPAQUE);
            }
            else if (cells[row][col] == CellState::Clicked)
            {
                SDL_SetRenderDrawColor(renderer, 220, 60, 60, SDL_ALPHA_OPAQUE);
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