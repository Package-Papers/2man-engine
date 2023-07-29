#pragma once
#ifndef INPUT_MOUSE
#define INPUT_MOUSE

#include <iostream>

#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/Window.hpp"

#include "input.hpp"

////////////////////////////////////
// Note(Appy): This file is for setting up a mouse input component.
// NOTE: WIP 7/27/03 -- Added basic mouse structure.

namespace tme
{

class Mouse : public tme::Input<Mouse>
{
    enum class State
    {
        NEUTRAL = 0,
        HELD,
        PRESSED
    };

  public:
    // This function will update all relevant fields according to the event.
    void impl(const sf::Event& event) noexcept;

  public:
    // Getters.
    [[nodiscard]] const sf::Vector2i get_position() const noexcept;
    [[nodiscard]] bool               is_held() const noexcept;
    [[nodiscard]] bool               is_pressed() const noexcept;

    void update_position(const sf::Window& window) noexcept;

  private:
    State        m_state;
    sf::Vector2i m_pos; // Position of the mouse.
};

} /* namespace tme */

#endif /* INPUT_MOUSE */