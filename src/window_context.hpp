#pragma once
#ifndef TME_WINDOW
#define TME_WINDOW

#include <SFML/Graphics.hpp>

namespace tme
{
using ushort = short unsigned;

struct WindowContext
{
    ushort      width;
    ushort      height;
    std::string title;
    ushort      m_target_fps;
};
} // namespace tme

#endif /* TME_WINDOW */
