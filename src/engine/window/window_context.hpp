#pragma once
#ifndef TME_WINDOW
#define TME_WINDOW

#include <string>

////////////////////////////////////
// Note(Appy): This file contains the struct which holds the configuration of the window.
// This object can be passed into the engine constructor to create the engine window as specified/
// desired.

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
