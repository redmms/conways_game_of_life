#pragma once
#include <SDL.h>
#include <memory>

namespace mmd
{
    class MainWindow
    {
        const int cell_size;
        const int width;
        const int height;
        std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> window;
        std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> renderer;
        std::unique_ptr<SDL_Texture, void (*)(SDL_Texture*)> background;
        std::unique_ptr<SDL_Texture, void (*)(SDL_Texture*)> foreground;

    public:
        MainWindow(int width_, int height_, int cell_size_);

        void Update(SDL_Texture& layer);
        int Width() const;
        int Height() const;
        int CellSize() const;
        SDL_Texture& Foreground();
        SDL_Renderer& Renderer();
        void SwitchTitle(bool pause_mode);
        void DrawLines();
    };
}
