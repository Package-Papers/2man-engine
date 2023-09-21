#pragma once
#include "engine/map/tile_type.hpp"
#include "engine/resource/resources.hpp"
#ifndef GAME_STATE
#define GAME_STATE

#include "engine/states/state.hpp"

#include "engine/input/input_system.hpp"

#include "engine/controller/controller.hpp"

#include "engine/ecs/ecs.hpp"

#include "engine/ecs/archetypes/lamp.hpp"
#include "engine/ecs/archetypes/player.hpp"

#include "engine/map/map.hpp"

#include "engine/ecs/systems/hitbox_system.hpp"
#include "engine/ecs/systems/physics_system.hpp"
#include "engine/ecs/systems/rendering_system.hpp"
#include "engine/ecs/systems/vicinity_system.hpp"

class GameState : public State
{
  public:
    GameState(StateStack& stack, Context context)
        : State(stack, context)
        , m_controller()
        , m_map(context, 30, 30, 1)
    {
        load_textures();
        m_controller.m_entity_manager = &m_entity_manager;

        LampArchetype lamp_factory{40.f, 40.f};
        auto          lamp        = lamp_factory.create(m_entity_manager);
        auto          lamp_hitbox = m_entity_manager.attach<Hitbox>(lamp);
        *lamp_hitbox              = {true, 20.f, 20.f, 0.f, 0.f};

        PlayerArchetype player_factory{100.f, 100.f};
        auto            player        = player_factory.create(m_entity_manager);
        auto            player_hitbox = m_entity_manager.attach<Hitbox>(player);
        *player_hitbox                = {true, 20.f, 20.f, 0.f, 0.f};

        m_controller.make_controller();

        register_systems();
        set_map();
    }

    void load_textures()
    {
        m_context.textures->load(textures::ID::Tile1, "assets/graphics/floor_tile.png");
    }

    void set_map()
    {
        auto [width, height] = m_map.dims();
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                m_map.at<0>(i, j).m_type = tile_types::FLOOR_TILE;
            }
        }
    }

    void register_systems()
    {
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

        const auto hb = m_systems.add_system<HitboxSystem>(m_context);
        m_systems.add_system<PhysicsSystem>(hb);

    }

    void draw()
    {
        m_map.draw(*m_context.window);
        draw_systems();
    }

    bool update(sf::Time dt)
    {
        m_controller.update();
        update_systems(dt);
        m_entity_manager.execute_actions();
        return true;
    }
    bool handle_event(const sf::Event& event)
    {
        return true;
    }

  private:
    Controller m_controller;
    Map m_map;

};

#endif /* GAME_STATE */