#pragma once
#ifndef TME_RENDERING_BUTTON_SYSTEM
#define TME_RENDERING_BUTTON_SYSTEM

#include <SFML/Graphics.hpp>

#include "engine/core/engine_context.hpp"
#include "engine/ecs/components.hpp"
#include "engine/ecs/entity_capture.hpp"
#include "engine/ecs/systems/system_base.hpp"
#include "engine/input/input_system.hpp"
#include "engine/states/state_base.hpp"

class RenderingButtonSystem : public SystemBase
{

  public:
    RenderingButtonSystem(EntityManager& em, Context context)
        : m_context(context)
        , m_rect({200, 200})
        , m_textures_cache(m_context.textures)
        , m_text(m_context.fonts->get(fonts::ID::Aerial))
        , m_input_system(tme::InputSystem::instance())
    {
        m_textures_cache.m_resource_holder = m_context.textures;
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

            auto mouse_pos = m_input_system->mouse.get_position();

            if (m_rect.getGlobalBounds().contains(
                    {static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)}))
            {
                button->state = Button::State::hover;

                if (m_input_system->mouse.is_pressed())
                {
                    button->state = Button::State::active;
                    button->action();
                }
            }

            else
            {
                button->state = Button::State::idle;
            }
        }
    }

    virtual void draw(EntityManager& em) override
    {
        for (auto e : EntityCapture<Button>(em))
        {
            auto [pos, button] = em.get<Position, Button>(e);
            m_text.setString(button->text);
            m_rect.setPosition({pos->x, pos->y});
            const auto texture1 = m_textures_cache.load(button->texture_ID);
            // const auto texture = &m_context.textures->get(button->texture_ID);
            m_rect.setTexture(texture1);
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
    Context                                  m_context;
    ResourceCache<sf::Texture, textures::ID> m_textures_cache;
    sf::RectangleShape                       m_rect;
    sf::Text                                 m_text;
    tme::InputSystem*                        m_input_system;
};

#endif /* TME_RENDERING_BUTTON_SYSTEM */