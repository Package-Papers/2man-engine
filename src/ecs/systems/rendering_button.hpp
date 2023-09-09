#pragma once
#ifndef TME_RENDERING_BUTTON_SYSTEM
#define TME_RENDERING_BUTTON_SYSTEM

#include <SFML/Graphics.hpp>

#include "../../state_base.hpp"
#include "../archetypes/button.hpp"
#include "../entity_capture.hpp"
#include "system_base.hpp"

class RenderingButtonsSystem : public SystemBase
{

  public:
    RenderingButtonsSystem(EntityManager& em, State::Context context)
        : m_context(context)
        , m_rect({200, 200})
        , m_text(m_context.fonts->get(fonts::ID::Aerial))
    {
        m_rect.setFillColor(sf::Color::Red);
        auto rectangle_bound = m_rect.getLocalBounds();
        m_rect.setOrigin({rectangle_bound.width / 2, rectangle_bound.height / 2});
    }

    virtual void update(EntityManager& em) override
    {
        for (auto e : EntityCapture<Button>(em))
        {
            auto [pos, button] = em.get<Position, Button>(e);
            m_rect.setPosition({pos->x, pos->y});
            auto mouse_pos = m_context.mouse->get_position();
            if (m_rect.getGlobalBounds().contains(
                    {static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)}))
            {
                button->state = Button::State::hover;

                if (m_context.mouse->is_pressed())
                {
                    button->state = Button::State::active;
                    button->action();
                }
            }

            else
            {
                button->state = Button::State::idle;
            }
            if (m_context.mouse->is_held())
                std::cout << "Hello\n";
        }
    }

    virtual void draw(EntityManager& em) override
    {
        for (auto e : EntityCapture<Button>(em))
        {
            auto [pos, button] = em.get<Position, Button>(e);
            m_text.setString(button->text);
            m_rect.setPosition({pos->x, pos->y});
            const auto texture = &m_context.textures->get(button->texture_ID);
            m_rect.setTexture(texture);
            m_context.window->draw(m_rect);

            switch (button->state)
            {
                case Button::State::hover:
                {
                    m_text.setFillColor(sf::Color::Blue);
                    break;
                }
                case Button::State::idle:
                {
                    m_text.setFillColor(sf::Color::White);
                    break;
                }
                case Button::State::active:
                {
                    m_text.setFillColor(sf::Color::Green);
                    break;
                }
            }

            auto text_bounds = m_text.getLocalBounds();
            m_text.setOrigin({text_bounds.width / 2, text_bounds.height / 2});

            m_text.setPosition({m_rect.getPosition().x, m_rect.getPosition().y - text_bounds.top});
            m_context.window->draw(m_text);
        }
    }

  private:
    State::Context     m_context;
    sf::RectangleShape m_rect;
    sf::Text           m_text;
};

#endif /* TME_RENDERING_BUTTON_SYSTEM */