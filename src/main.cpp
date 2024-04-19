#include <SDL.h>
#include "mainwindow.h"
#include "board.h"
#include <vector>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::vector<std::string> str_args(argv, argv + argc);
    const int cell_size = 10;
    int speed = 9;
    int window_width = 500;
    int window_height = 500;
    int arg1, arg2, arg3;
    const char* parameters_description =
        "NOTE: you can use 3 int command line arguments: speed from 1"
        " to 10, window width from 100 to max window width, window height"
        " from 100 to max window height, window size arguments should be"
        " multiples of 10, like 110, 120, etc.";
    try {
        if (argc > 1) {
            arg1 = std::stoi(str_args[1]) - 1;
            if (arg1 < 0 || arg1 > 9) {
                std::cerr << "WARNING: speed should be from 1 to 10" << std::endl;
            }
            else {
                speed = arg1;
            }
        }
        if (argc > 2) {
            arg2 = std::stoi(str_args[2]);
            if (arg2 < 100) {
                std::cerr << "WARNING: minimum window width is 100" << std::endl;
            }
            else {
                window_width = arg2;
            }
        }
        if (argc > 3) {
            arg3 = std::stoi(str_args[3]);
            if (arg3 < 100) {
                std::cerr << "WARNING: minimum window height is 100" << std::endl;
            }
            else {
                window_height = arg3;
            }
        }
        if (window_width % cell_size || window_height % cell_size) {
            std::cerr << "WARNING: size arguments should be a multiple of 10, it will"
                "be shrinked to the nearest appropriate size" << std::endl;
            window_width -= window_width % cell_size;
            window_height -= window_height % cell_size;
        }
        if (SDL_Init(SDL_INIT_EVERYTHING)) {
            std::cerr << "ERROR: couldn't initialize SDL: " << SDL_GetError() << std::endl;
            std::cin.get();
            return 2;
        }
        SDL_DisplayMode dm;
        if (SDL_GetDesktopDisplayMode(0, &dm)) {
            std::cerr << "WARNING: couldn't get display info. Maximum window size"
                "will not be considered: " << SDL_GetError() << std::endl;
        }
        else {
            if (window_width > dm.w) {
                window_width = dm.w;
                std::cerr << "WARNING: maximum window width is: " << dm.w << std::endl;
            }
            if (window_height > dm.h) {
                window_height = dm.h;
                std::cerr << "WARNING: maximum window height is: " << dm.h << std::endl;
            }
        }
        if (argc < 4) {
            std::cerr << parameters_description << std::endl;
        }
        std::cout << "Controls are:" << std::endl
            << "space - pause / run" << std::endl
            << "c - clear the board, works in pause mode only" << std::endl
            << "esc - close the game" << std::endl
            << "Pause and click on a cell to give it live or kill" << std::endl;

        mmd::MainWindow window(window_width, window_height, cell_size);
        mmd::Board board(window);
        uint64_t delta = 2000 - speed * 200;
        bool pause_mode = true;
        bool main_loop = true;
        SDL_AddTimer(static_cast<uint32_t>(delta),
            [](uint32_t interval, void*) {
                SDL_Event event{ .type = SDL_USEREVENT };
                mmd::LogError(!SDL_PushEvent(&event));
                return interval;
            },
            nullptr);
        while (main_loop) {
            SDL_Event event;
            if (!SDL_WaitEvent(&event)) {
                std::cerr << SDL_GetError() << std::endl;
                return 3;
            }
            else if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE) {
                main_loop = false;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                pause_mode = !pause_mode;
                window.SwitchTitle(pause_mode);
            }
            else if (pause_mode) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_c) {
                    board.Clear();
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    board.ToggleCell({ event.button.x / cell_size, event.button.y / cell_size });
                }
            }
            else if (!pause_mode) {
                if (event.type == SDL_USEREVENT) {
                    board.NewGeneration();
                }
            }
        }
    }
    catch (std::exception& err) {
        std::cerr << "ERROR: invalid input: " << err.what() << std::endl;
        std::cerr << "Arguments were:" << std::endl;
        for (const auto& str : str_args) {
            std::cerr << str << std::endl;
        }
        std::cerr << parameters_description << std::endl;
        std::cin.get();
        return 1;
    }
    return 0;
}
