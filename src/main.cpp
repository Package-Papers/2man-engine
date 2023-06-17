#include "common.hpp"
#include "engine.hpp"

/**
 * Define settings here, else load default.
 */

// #define TME_WINDOW_WIDTH        1920
// #define TME_WINDOW_HEIGH        1080
// #define TME_WINDOW_FRAMES_LIM   144
#define TME_WINDOW_FULLSCREEN 1
// #define TME_VERTICAL_SYNC       0

#include "settings.hpp"

int main()
{
    const tme::WindowContext ctx = {
        TME_WINDOW_WIDTH,
        TME_WINDOW_HEIGHT,
        TME_WINDOW_TITLE,
        TME_WINDOW_FRAMES_LIM,
        int_to_bool(TME_VERTICAL_SYNC),
        int_to_bool(TME_WINDOW_FULLSCREEN),
    };

    tme::Engine engine(ctx);
    engine.run();
    return 0;
}