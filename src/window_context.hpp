#pragma once
#ifndef TME_WINDOW
#define TME_WINDOW

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace tme
{
using ushort = short unsigned;

struct WindowContext
{
    ushort      width;
    ushort      height;
    std::string title;
    ushort      target_fps;
    bool        vsync;
    bool        fullscreen;
};
} // namespace tme

#endif /* TME_WINDOW */
