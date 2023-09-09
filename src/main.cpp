#define DOCTEST_CONFIG_IMPLEMENT
#include "common.hpp"
#include "engine.hpp"

/**
 * Settings can be defined in 'settings.json'.
 */

#include "settings.hpp"

void doctest_run(int argc, char** argv)
{
    doctest::Context ctx;
    ctx.setOption("abort-after", 1);
    ctx.applyCommandLine(argc, argv);
    ctx.setOption("no-breaks", true);
    int res = ctx.run();

    if (ctx.shouldExit())
    {
        exit(res);
    }
}

int main(int argc, char** argv)
{
    LOG("Running - Debug\n");

#ifdef DOCTEST_RUN
    LOG("Running tests...\n");
    doctest_run(argc, argv);
    return 0;
#else
    LOG("Tests skipped...\n");
#endif
    // Try running doctest.

    const tme::WindowContext default_ctx = {
        TME_WINDOW_WIDTH,
        TME_WINDOW_HEIGHT,
        TME_WINDOW_TITLE,
        TME_WINDOW_FRAMES_LIM,
        int_to_bool(TME_VERTICAL_SYNC),
        int_to_bool(TME_WINDOW_FULLSCREEN),
    };

    auto loaded_settings = load_settings("meta/settings.json");

    tme::Engine engine(loaded_settings.value_or(default_ctx));
    engine.run();
    return 0;
}