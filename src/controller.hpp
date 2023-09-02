#pragma
#ifndef TME_CONTROLLER
#define TME_CONTROLLER
#include "ecs/components.hpp"
#include "ecs/entity_capture.hpp"
#include "keyboard.hpp"
#include <vector>

class EntityManager;

class Controller
{
  public:
    void make_controller();
    void update();

    EntityManager*        m_entity_manager;
    tme::Keyboard*        m_keyboard;
    std::vector<EntityID> m_controllers;
};

inline void Controller::make_controller()
{
    for (EntityID e : EntityCapture<Controllable>(*m_entity_manager))
    {
        m_controllers.push_back(e);
    }
}

inline void Controller::update()
{
    for (auto e : m_controllers)
    {
        auto pos = m_entity_manager->get<Position>(e);
        if (m_keyboard->is_key_pressed(sf::Keyboard::Key::S))
        {
            pos->y += 0.1f;
        }
        else if (m_keyboard->is_key_pressed(sf::Keyboard::Key::W))
        {
            pos->y -= 0.1f;
        }

        else if (m_keyboard->is_key_pressed(sf::Keyboard::Key::A))
        {
            pos->x -= 0.1f;
        }

        else if (m_keyboard->is_key_pressed(sf::Keyboard::Key::D))
        {
            pos->x += 0.1f;
        }
    }
}

#endif /* TME_CONTROLLER */