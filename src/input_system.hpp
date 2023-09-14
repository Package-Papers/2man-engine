
#pragma once
#ifndef TME_INPUT_SYSTEM
#define TME_INPUT_SYSTEM

#include "keyboard.hpp"
#include "mouse.hpp"

namespace tme
{
class InputSystem
{
  private:
    static InputSystem* singleton;

  public:
    static InputSystem* instance();
    InputSystem()
    {
        singleton = this;
    }
    ~InputSystem()
    {
        singleton = nullptr;
    }

  private:
    Keyboard m_keyboard;
    Mouse    m_mouse;
};
} // namespace tme

inline tme::InputSystem* tme::InputSystem::singleton = nullptr;

inline tme::InputSystem* tme::InputSystem::instance()
{
    return singleton;
}

#endif /* TME_INPUT_SYSTEM */
