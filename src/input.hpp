
#pragma once
#ifndef TME_INPUT
#define TME_INPUT

#include "SFML/Window/Event.hpp"

////////////////////////////////////
// Note(Appy): Interface for input devices.

#ifndef __cpp_explicit_this_parameter // Handle traditional syntax.

template <class Derived>
class Input
{
    void handle_event(const sf::Event& event) noexcept
    {
        (static_cast<Derived*>(this))->impl(event);
    }
};

#else // C++23 syntax

class Input
{
    void handle_event(this auto&& self) noexcept
    {
        self.impl();
    }
};

#endif

#endif /* TME_INPUT */
