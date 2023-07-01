#include "common.hpp"
#include "engine.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

/**
 * Settings can be defined in 'settings.json'.
 */

#include "settings.hpp"

int main()
{
    const tme::WindowContext default_ctx = {
        TME_WINDOW_WIDTH,
        TME_WINDOW_HEIGHT,
        TME_WINDOW_TITLE,
        TME_WINDOW_FRAMES_LIM,
        int_to_bool(TME_VERTICAL_SYNC),
        int_to_bool(TME_WINDOW_FULLSCREEN),
    };

    auto loaded_settings = load_settings("settings.json");

    tme::Engine engine(loaded_settings.value_or(default_ctx));
    engine.run();
    return 0;
}