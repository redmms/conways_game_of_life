#include "local_types.h"
#include <SDL.h>
#include <iostream>

namespace mmd {
    void LogError(int error_code) {
        if (error_code) {
            std::cerr << SDL_GetError() << std::endl;
        }
    }
}