#pragma once
#ifndef INPUT_MOUSE
#define INPUT_MOUSE

#include "SFML/Window/Event.hpp"
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

private:
  State m_state;
  sf::Vector2f m_pos; // Position of the mouse.

public:
  // This function will update all relevant fields according to the event.
  void impl(const sf::Event& event) noexcept
  {
    // If it was previous set as pressed, we have to reset it to neutral.
    if (m_state == State::PRESSED) m_state = State::NEUTRAL;

    State new_state = State::NEUTRAL;

    switch(event.type)
    {
      break; case sf::Event::MouseButtonPressed:
      {
        // The mouse is being held
        if (m_state != State::HELD && event.mouseButton.button == sf::Mouse::Left)
        {
          new_state = State::HELD;
        }
      }
      break; case sf::Event::MouseButtonReleased:
      {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
          new_state = State::PRESSED;
        }
      }
      // TODO: Update position of the mouse on state MOVED.
      break; default: return; // Keep the same state;
    }
    m_state = new_state;
  }
public:
  [[nodiscard]] const sf::Vector2f get_mouse_position() const noexcept
  {
    return m_pos;
  }

  [[nodiscard]] const bool is_held() const noexcept
  {
    return m_state == State::HELD;
  }
  [[nodiscard]] const bool is_pressed() const noexcept
  {
    return m_state == State::PRESSED;
  }
};

} /* namespace tme */

#endif  /* INPUT_MOUSE */