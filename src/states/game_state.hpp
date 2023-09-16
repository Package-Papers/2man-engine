#pragma once
#ifndef GAME_STATE
#define GAME_STATE

#include "states/state.hpp"

#include "core/input/input_system.hpp"

#include "../controller.hpp"

#include "../ecs/ecs.hpp"

#include "../ecs/archetypes/lamp.hpp"
#include "../ecs/archetypes/player.hpp"

#include "../ecs/systems/rendering_system.hpp"
#include "../ecs/systems/vicinity_system.hpp"

class GameState : public State
{
  public:
    GameState(StateStack& stack, Context context)
        : State(stack, context)
        , m_controller()
    {
        m_controller.m_entity_manager = &m_entity_manager;

        LampArchetype lamp_factory{10.f, 10.f};
        auto          lamp = lamp_factory.create(m_entity_manager);

        LampArchetype lamp_factory2{20.f, 20.f};
        auto          lamp2 = lamp_factory2.create(m_entity_manager);

        PlayerArchetype player_factory{100.f, 100.f};
        auto            player = player_factory.create(m_entity_manager);

        m_controller.make_controller();

        auto check_interactable =
            [=, this](EntityManager* m_entity_manager, EntityID e1, EntityID e2)
        {
            auto input_system = tme::InputSystem::instance();
            if (input_system->keyboard.is_key_pressed(sf::Keyboard::Key::E))
            {
                auto interactable = m_entity_manager->get<Interactable>(e1);
                m_entity_manager->get<Interactable>(e1)->action(m_entity_manager, e1, e2);
            }
        };

        auto vct = m_systems.add_system<VicinitySystem>();
        vct->listen(check_interactable);

        m_systems.add_system<RenderingSystem>(m_context);
    }

    void draw()
    {
        draw_systems();
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
    Controller m_controller;
};

#endif /* GAME_STATE */