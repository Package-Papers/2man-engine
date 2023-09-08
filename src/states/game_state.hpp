#pragma once
#ifndef GAME_STATE
#define GAME_STATE

#include "../controller.hpp"
#include "../ecs/archetypes/lamp.hpp"
#include "../ecs/archetypes/player.hpp"
#include "../ecs/ecs.hpp"
#include "../ecs/systems/rendering_system.hpp"
#include "../ecs/systems/vicinity_system.hpp"
#include "../state.hpp"
#include "../map/map.hpp"

class GameState : public State
{
  public:
    GameState(StateStack& stack, Context context)
        : State(stack, context)
        , m_controller()
        , m_map(m_context, 10, 10)
    {
        m_context.textures->load(textures::ID::Tile1, "assets/graphics/floor_tile.png");

        init_map();

        m_controller.m_entity_manager = &m_entity_manager;
        m_controller.m_keyboard       = m_context.keyboard;

        LampArchetype lamp_factory{10.f, 10.f};
        auto          lamp = lamp_factory.create(m_entity_manager);

        LampArchetype lamp_factory2{20.f, 20.f};
        auto          lamp2 = lamp_factory2.create(m_entity_manager);

        PlayerArchetype player_factory{100.f, 100.f};
        auto            player = player_factory.create(m_entity_manager);

        m_controller.make_controller();

        auto check_interactable = [=, this](EntityManager* m_entity_manager, EntityID e1, EntityID e2)
        {
            if (m_context.keyboard->is_key_pressed(sf::Keyboard::Key::E))
            {
                auto interactable = m_entity_manager->get<Interactable>(e1);
                m_entity_manager->get<Interactable>(e1)->action(m_entity_manager, e1, e2);
            }
        };

        auto vct = m_systems.add_system<VicinitySystem>();
        vct->listen(check_interactable);

        m_systems.add_system<RenderingSystem>(m_context);
    }

    void init_map()
    {
        auto [width, height] = m_map.dims();

        for (auto y = 5; y < width; y++)
        {
            for (auto x = 5; x < height; x++)
            {
                m_map.at<0>(y, x) = {{}, {0, 0}, textures::ID::Tile1};
            }
        }
    }

    void draw()
    {
        m_map.draw(*m_context.window);
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
    Map        m_map;
};

#endif /* GAME_STATE */