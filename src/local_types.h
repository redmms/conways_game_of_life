#pragma once

namespace mmd {
    struct coord {
        int x = 0;
        int y = 0;
        coord operator+ (const coord& add) {
            return { x + add.x, y + add.y };
        }
    };

    void LogError(bool error);
}