#include "board.h"
#include "mainwindow.h"
#include <SDL.h>
#include <iostream>

namespace mmd
{
    const std::vector<coord> Board::neighbour_coords =
    {
        { -1, 1 },  { 0, 1 },  { 1, 1 },
        { -1, 0 },             { 1, 0 },
        { -1, -1 }, { 0, -1 }, { 1, -1 }
    };

    Board::Board(MainWindow& window_) :
        width(window_.Width() / window_.CellSize()),
        height(window_.Height() / window_.CellSize()),
        current_gen(width, std::vector<uint8_t>(height, 0)),
        new_gen(width, std::vector<uint8_t>(height, 0)),
        cell_size(window_.CellSize()),
        cell_size1(cell_size - 1),
        window(window_),
        foreground(window_.Foreground()),
        renderer(window_.Renderer())
    {
    }

    int Board::CountNeighbours(coord mid, bool color) const
    {
        int count = 0;
        for (const coord& add : neighbour_coords) {
            coord neighbour = mid + add;
            if (ValidCoord(neighbour)) {
                count += GetColor(neighbour) == color;
            }
        }
        return count;
    }

    void Board::NewGeneration() {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                coord cell = { x, y };
                bool color = GetColor(cell);
                int neighbours_n = CountNeighbours(cell, true);
                bool new_color = color;
                if (color && (neighbours_n > 3 || neighbours_n < 2)) {
                    new_color = false;
                }
                else if (!color && neighbours_n == 3) {
                    new_color = true;
                }
                new_gen[x][y] = new_color;
                DrawCell(cell, new_color);
            }
        }
        std::swap(new_gen, current_gen);
        window.Update(foreground);
    }

    void Board::DrawCell(coord xy, bool color)
    {
        LogError(SDL_SetRenderTarget(&renderer, &foreground));
        if (color) {
            LogError(SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255));
        }
        else {
            LogError(SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255));
        }
        SDL_Rect foreground_rect{ xy.x * cell_size + 1, xy.y * cell_size + 1, 
            cell_size1, cell_size1 };
        LogError(SDL_RenderFillRect(&renderer, &foreground_rect));
    }

    void Board::Clear()
    {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (current_gen[x][y]) {
                    current_gen[x][y] = false;
                    DrawCell({ x, y }, false);
                }
            }
        }
        window.Update(foreground);
    }

    void Board::ToggleCell(coord xy)
    {
        bool new_color = !GetColor(xy);
        SetColor(xy, new_color);
        DrawCell(xy, new_color);
        window.Update(foreground);
    }

    bool Board::GetColor(coord xy) const
    {
        if (!ValidCoord(xy)) {
            throw std::out_of_range("Board::GetColor coord argument is invalid");
        }
        return static_cast<bool>(current_gen[xy.x][xy.y]);
    }

    void Board::SetColor(coord xy, bool color)
    {
        if (!ValidCoord(xy)) {
            throw std::out_of_range("Board::SetColor coord argument is invalid");
        }
        current_gen[xy.x][xy.y] = static_cast<uint8_t>(color);
    }

    bool Board::ValidCoord(coord xy) const
    {
        return xy.x >= 0 && xy.x < width && xy.y >= 0 && xy.y < height;
    }
}
