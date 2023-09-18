#include "mouse.hpp"

void tme::Mouse::impl(const sf::Event& event) noexcept
{
    // If it was previous set as pressed, we have to reset it to neutral.
    if (m_state == State::PRESSED)
        m_state = State::NEUTRAL;

    State new_state = State::NEUTRAL;

    switch (event.type)
    {
        break;
        case sf::Event::MouseButtonPressed:
        {
            // The mouse is being held
            if (m_state != State::HELD && event.mouseButton.button == sf::Mouse::Left)
            {
                new_state = State::HELD;
            }
        }
        break;
        case sf::Event::MouseButtonReleased:
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                new_state = State::PRESSED;
            }
        }
        break;
        default:
            return; // Keep the same state;
    }
    m_state = new_state;
}

const sf::Vector2i tme::Mouse::get_position() const noexcept
{
    return m_pos;
}

void tme::Mouse::update_position(const sf::Window& window) noexcept
{
    // Set position
    m_pos = sf::Mouse::getPosition(window);
}

bool tme::Mouse::is_pressed() const noexcept
{
    return m_state == State::PRESSED;
}

bool tme::Mouse::is_held() const noexcept
{
    return m_state == State::HELD;
}
