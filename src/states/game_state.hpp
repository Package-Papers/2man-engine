#pragma once
#ifndef GAME_STATE
#define GAME_STATE

#include "../controller.hpp"
#include "../state.hpp"

class GameState : public State
{
  public:
    GameState(StateStack& stack, Context context)
        : State(stack, context)
        , m_shape(15)
        , m_controller()
    {
        m_controller.m_entity_manager = &m_entity_manager;
        m_controller.m_keyboard       = m_context.keyboard;
        m_shape.setFillColor(sf::Color::White);

        auto e1 = m_entity_manager.create_entity();
        m_entity_manager.attach<Controllable>(e1);
        m_entity_manager.attach<Position>(e1);
        m_controller.make_controller();
    }

    void draw()
    {
        for (EntityID e : EntityCapture<Controllable>(m_entity_manager))
        {
            auto pos = m_entity_manager.get<Position>(e);

            m_shape.setPosition({pos->x, pos->y});
            m_context.window->draw(m_shape);
        }
    }
    bool update(sf::Time dt)
    {
        m_controller.update();
    }
    bool handle_event(const sf::Event& event)
    {
        return true;
    }

  private:
    sf::CircleShape m_shape;
    Controller      m_controller;
    EntityManager   m_entity_manager;
};

#endif /* GAME_STATE */