#pragma once
#ifndef GAME_STATE
#define GAME_STATE

#include "../controller.hpp"
#include "../ecs/ecs_all.hpp"
#include "../state.hpp"

class GameState : public State
{
  public:
    GameState(StateStack& stack, Context context)
        : State(stack, context)
        , m_shape(15)
        , m_rectangle_shape({10, 10})
        , m_controller()
    {
        m_controller.m_entity_manager = &m_entity_manager;
        m_controller.m_keyboard       = m_context.keyboard;
        m_shape.setFillColor(sf::Color::White);
        m_rectangle_shape.setFillColor(sf::Color::Red);

        LampArchetype lamp_factory{10.f, 10.f};
        auto          lamp = lamp_factory.create(m_entity_manager);

        LampArchetype lamp_factory2{20.f, 20.f};
        auto          lamp2 = lamp_factory2.create(m_entity_manager);

        PlayerArchetype player_factory{100.f, 100.f};
        auto            player = player_factory.create(m_entity_manager);

        m_controller.make_controller();

        auto check_interactable = [=](EntityManager* m_entity_manager, EntityID e1, EntityID e2)
        {
            if (m_context.keyboard->is_key_pressed(sf::Keyboard::Key::E))
            {
                std::cout << "interactable\n";
                auto interactable = m_entity_manager->get<Interactable>(e1);
                if (interactable->action == nullptr)
                    std::cout << "empty!\n";
                m_entity_manager->get<Interactable>(e1)->action(m_entity_manager, e1, e2);
                std::cout << "interactable2\n";
            }
        };
        auto vct = new VicinitySystem;
        vct->listen(check_interactable);
        m_systems.emplace_back(vct);
    }

    void draw()
    {
        m_rectangle_shape.setFillColor(sf::Color::White);

        RenderingSystem

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
    bool update(sf::Time dt)
    {
        m_controller.update();
        update_systems();
        m_entity_manager.execute_actions();
        return true;
    }
    bool handle_event(const sf::Event& event)
    {
        return true;
    }

  private:
    sf::RectangleShape m_rectangle_shape;
    sf::CircleShape    m_shape;
    Controller         m_controller;
};

#endif /* GAME_STATE */