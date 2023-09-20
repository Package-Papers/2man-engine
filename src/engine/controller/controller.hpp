#pragma
#ifndef TME_CONTROLLER
#define TME_CONTROLLER

#include <vector>

#include "engine/ecs/components.hpp"
#include "engine/ecs/entity_capture.hpp"
#include "engine/input/input_system.hpp"

class EntityManager;

class Controller
{
  public:
    void get_interactables();
    void make_controller();
    void update();

    EntityManager*        m_entity_manager;
    std::vector<EntityID> m_controllers;
    std::vector<EntityID> m_interactables;
};

inline void Controller::make_controller()
{
    for (EntityID e : EntityCapture<Controllable>(*m_entity_manager))
    {
        m_controllers.push_back(e);
    }
}

inline void Controller::get_interactables()
{
    for (EntityID e : EntityCapture<Interactable>(*m_entity_manager))
    {
        m_interactables.push_back(e);
    }
}

inline void Controller::update()
{
    auto keyboard = &(tme::InputSystem::instance()->keyboard);
    for (auto e : m_controllers)
    {
        auto vel      = m_entity_manager->get<Velocity>(e);

        // This should invoke a warning!
        if (vel == nullptr) continue;


        if (keyboard->is_key_pressed(sf::Keyboard::Key::S))
        {
            auto m_action = m_entity_manager->create_action();
            m_action->func = [=](EntityManager* m_entity_manager, EntityID e) { vel->apply(0.f, 5.f); };
            m_action->targets.push_back(e);
        }

        if (keyboard->is_key_pressed(sf::Keyboard::Key::W))
        {
            auto m_action = m_entity_manager->create_action();
            m_action->func = [=](EntityManager* m_entity_manager, EntityID e) { vel->apply(0.f, -5.f); };
            m_action->targets.push_back(e);
        }

        if (keyboard->is_key_pressed(sf::Keyboard::Key::A))
        {
            auto m_action = m_entity_manager->create_action();
            m_action->func = [=](EntityManager* m_entity_manager, EntityID e) { vel->apply(-5.f, 0.f); };
            m_action->targets.push_back(e);
        }

        if (keyboard->is_key_pressed(sf::Keyboard::Key::D))
        {
            auto m_action = m_entity_manager->create_action();
            m_action->func = [=](EntityManager* m_entity_manager, EntityID e) { vel->apply(5.f, 0.f); };
            m_action->targets.push_back(e);
        }
    }
}

#endif /* TME_CONTROLLER */