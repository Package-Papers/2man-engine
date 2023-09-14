#pragma
#ifndef TME_CONTROLLER
#define TME_CONTROLLER

#include <vector>

#include "ecs/components.hpp"
#include "ecs/entity_capture.hpp"
#include "input_system.hpp"

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
        auto pos      = m_entity_manager->get<Position>(e);
        auto m_action = m_entity_manager->create_action();
        if (keyboard->is_key_pressed(sf::Keyboard::Key::S))
        {
            m_action->func = [=](EntityManager* m_entity_manager, EntityID e) { pos->y += 1.0f; };
            m_action->targets.push_back(e);
        }
        else if (keyboard->is_key_pressed(sf::Keyboard::Key::W))
        {
            m_action->func = [=](EntityManager* m_entity_manager, EntityID e) { pos->y -= 1.0f; };
            m_action->targets.push_back(e);
        }

        else if (keyboard->is_key_pressed(sf::Keyboard::Key::A))
        {
            m_action->func = [=](EntityManager* m_entity_manager, EntityID e) { pos->x -= 1.0f; };
            m_action->targets.push_back(e);
        }

        else if (keyboard->is_key_pressed(sf::Keyboard::Key::D))
        {
            m_action->func = [=](EntityManager* m_entity_manager, EntityID e) { pos->x += 1.0f; };
            m_action->targets.push_back(e);
        }
    }
}

#endif /* TME_CONTROLLER */