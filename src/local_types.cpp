#include "local_types.h"
#include <SDL.h>
#include <iostream>

namespace mmd {
    void LogError(bool error) {
        if (error) {
            std::cerr << SDL_GetError() << std::endl;
        }
    }
}