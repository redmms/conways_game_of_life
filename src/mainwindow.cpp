#include "mainwindow.h"
#include "local_types.h"
#include <SDL.h>
#include <iostream>

namespace mmd
{
    MainWindow::MainWindow(int width_, int height_, int cell_size_) :
        cell_size(cell_size_),
        width(width_ - (width_ % cell_size)),
        height(height_ - (height_ % cell_size)),
        window(nullptr, &SDL_DestroyWindow),
        renderer(nullptr, &SDL_DestroyRenderer),
        background(nullptr, &SDL_DestroyTexture),
        foreground(nullptr, &SDL_DestroyTexture)
    {
        window.reset(SDL_CreateWindow(
            "Conway's game of life",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN
        ));
        if (!window) {
            throw std::runtime_error("In MainWindow constructor: couldn't"
                " initialize window");
        }
        renderer.reset(SDL_CreateRenderer(window.get(), -1, 0));
        if (!renderer) {
            throw std::runtime_error("In MainWindow constructor: couldn't"
                " initialize renderer");
        }
        background.reset(SDL_CreateTexture(
            renderer.get(),
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            width,
            height
        ));
        if (!background) {
            throw std::runtime_error("In MainWindow constructor: couldn't"
                " initialize background");
        }
        foreground.reset(SDL_CreateTexture(
            renderer.get(),
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            width,
            height
        ));
        if (!foreground) {
            throw std::runtime_error("In MainWindow constructor: couldn't"
                " initialize foreground");
        }
        LogError(SDL_SetTextureBlendMode(foreground.get(), SDL_BLENDMODE_BLEND));
        DrawLines();
        Update(*background);
    }

    void MainWindow::DrawLines()
    {
        LogError(SDL_SetRenderTarget(renderer.get(), background.get()));
        LogError(SDL_SetRenderDrawColor(renderer.get(), 100, 100, 100, 255));
        for (int x = cell_size; x < width; x += cell_size) {
            LogError(SDL_RenderDrawLine(renderer.get(), x, 0, x, height));
        }
        for (int y = cell_size; y < height; y += cell_size) {
            LogError(SDL_RenderDrawLine(renderer.get(), 0, y, width, y));
        }
    }

    void MainWindow::SwitchTitle(bool pause_mode)
    {
        if (pause_mode) {
            SDL_SetWindowTitle(window.get(), "Conway's game of life: Pause");
        }
        else {
            SDL_SetWindowTitle(window.get(), "Conway's game of life: Running");
        }
    }

    void MainWindow::Update(SDL_Texture& layer)
    {
        LogError(SDL_SetRenderTarget(renderer.get(), 0));
        LogError(SDL_RenderCopy(renderer.get(), &layer, 0, 0));
        SDL_RenderPresent(renderer.get());
    }

    int MainWindow::Width() const
    {
        return width;
    }

    int MainWindow::Height() const
    {
        return height;
    }

    SDL_Texture& MainWindow::Foreground()
    {
        return *foreground;
    }

    SDL_Renderer& MainWindow::Renderer()
    {
        return *renderer;
    }

    int MainWindow::CellSize() const
    {
        return cell_size;
    }
}
