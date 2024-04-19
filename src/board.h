#pragma once
#include "local_types.h"
#include <SDL.h>
#include <vector>

namespace mmd {
    class MainWindow;
    class Board {
        int width;
        int height;
        std::vector<std::vector<uint8_t>> current_gen;
        std::vector<std::vector<uint8_t>> new_gen;
        static const std::vector<coord> neighbour_coords;
        MainWindow& window;
        SDL_Texture& foreground;
        SDL_Renderer& renderer;
        const int cell_size;
        const int cell_size1;

        int CountNeighbours(coord mid, bool color) const;
        bool ValidCoord(coord xy) const;

    public:
        Board(MainWindow& window);

        void NewGeneration();
        void ToggleCell(coord xy);
        void Clear();
        void DrawCell(coord xy, bool color);
        bool GetColor(coord xy) const;
        void SetColor(coord xy, bool color);
    };
}
