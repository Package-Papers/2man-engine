
#pragma once
#ifndef TME_INPUT_SYSTEM
#define TME_INPUT_SYSTEM

#include <iostream>

#include "input.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"

namespace tme
{
#ifndef __cpp_explicit_this_parameter
class InputSystem : public tme::Input<InputSystem>
#else
class InputSystem : public Input
#endif
{
  private:
    static InputSystem* singleton;

  public:
    static InputSystem* instance();

    void impl(const sf::Event& event) noexcept;
    InputSystem()
    {
        std::cout << "Made\n";
        singleton = this;
    }
    ~InputSystem()
    {
        std::cout << "Destroyed\n";
        singleton = nullptr;
    }

    // todo: this should become private later.
    //       add interfaces with input devices.
  public:
    Keyboard keyboard;
    Mouse    mouse;
};
} // namespace tme

inline tme::InputSystem* tme::InputSystem::singleton = nullptr;

inline tme::InputSystem* tme::InputSystem::instance()
{
    return singleton;
}

inline void tme::InputSystem::impl(const sf::Event& event) noexcept
{
    // Dispatch events to input devices
    mouse.handle_event(event);
    keyboard.handle_event(event);
}

#endif /* TME_INPUT_SYSTEM */
