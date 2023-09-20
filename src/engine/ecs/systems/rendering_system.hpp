#pragma once
#ifndef TME_RENDERING_SYSTEM
#define TME_RENDERING_SYSTEM

#include <SFML/Graphics.hpp>

#include "engine/ecs/entity_capture.hpp"
#include "engine/states/state_base.hpp"
#include "system_base.hpp"

class RenderingSystem : public SystemBase
{

  public:
    RenderingSystem(Context context)
        : m_context(context)
        , m_shape(15)
        , m_rectangle_shape({10, 10})
    {
        m_rectangle_shape.setFillColor(sf::Color::White);
    }

    virtual void update(EntityManager& m_entity_manager, sf::Time) override
    {
    }

    virtual void draw(EntityManager& m_entity_manager) override
    {
        for (EntityID e : EntityCapture<Controllable>(m_entity_manager))
        {
            auto pos = m_entity_manager.get<Position>(e);

            m_shape.setPosition({pos->x, pos->y});
            m_context.window->draw(m_shape);
        }
        for (EntityID e : EntityCapture<Interactable>(m_entity_manager))
        {
            auto pos = m_entity_manager.get<Position>(e);
            auto col = m_entity_manager.get<Colour>(e);
            m_rectangle_shape.setPosition({pos->x, pos->y});
            m_rectangle_shape.setFillColor(sf::Color(col->r, col->g, col->b));
            m_context.window->draw(m_rectangle_shape);
        }
    }

  private:
    sf::RectangleShape m_rectangle_shape;
    sf::CircleShape    m_shape;
    Context            m_context;
};

#endif /* TME_RENDERING_SYSTEM */