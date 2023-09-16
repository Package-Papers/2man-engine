#pragma once
#ifndef TME_INPUT_KEYBOARD
#define TME_INPUT_KEYBOARD

#include <unordered_map>

#ifndef NDEBUG
#include <iostream>
#endif

#include <SFML/Window/Keyboard.hpp>

#include "core/structures/bitmask.hpp"
#include "core/input/input.hpp"

namespace tme
{
#ifndef __cpp_explicit_this_parameter
class Keyboard : public tme::Input<Keyboard>
#else
class Keyboard : public Input
#endif
{
  public:
    using Keybinds = std::unordered_map<std::string, sf::Keyboard::Key>;

  public:
    void set_keybinds(const Keybinds& kb)
    {
        m_supported_keys = kb;
    }

    void impl(const sf::Event&) noexcept
    {
        // Doesn't need to do anything...
    }

    // Real time updates.
    void update()
    {
        m_last_frame_keys.set_mask(m_this_frame_keys);

        for (auto const& [keyword, keynum] : m_supported_keys)
        {
            if (keynum == sf::Keyboard::Key::Unknown)
                continue;
            m_this_frame_keys.set_bit(static_cast<int>(keynum), sf::Keyboard::isKeyPressed(keynum));
        }

#ifndef NDEBUG

        for (auto const& [keyword, keynum] : m_supported_keys)
        {
            if (keynum == sf::Keyboard::Key::Unknown)
                continue;
            else if (is_key_down(keynum))
            {
                std::cout << "Key down: " << keyword << '\n';
            }
            else if (is_key_released(keynum))
            {
                std::cout << "Key released: " << keyword << '\n';
            }
        }
#endif
    }

    // Currently pressed.
    bool is_key_pressed(sf::Keyboard::Key key)
    {
        return m_this_frame_keys.get_bit(static_cast<int>(key));
    }

    // Key was just held.
    bool is_key_down(sf::Keyboard::Key key)
    {
        auto last_frame = m_last_frame_keys.get_bit(static_cast<int>(key));
        auto this_frame = m_this_frame_keys.get_bit(static_cast<int>(key));

        return this_frame && !last_frame;
    }

    // Key was just released.
    bool is_key_released(sf::Keyboard::Key key)
    {
        auto last_frame = m_last_frame_keys.get_bit(static_cast<int>(key));
        auto this_frame = m_this_frame_keys.get_bit(static_cast<int>(key));

        return !this_frame && last_frame;
    }

  private:
    Keybinds         m_supported_keys;
    tme::Bitmask<64> m_last_frame_keys;
    tme::Bitmask<64> m_this_frame_keys;
};
} /* namespace tme */

#endif /* TME_INPUT_KEYBOARD */
