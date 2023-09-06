#pragma once
#ifndef GAME_STATE
#define GAME_STATE

#include "../controller.hpp"
#include "../state.hpp"

struct VicinitySystem
{
    using Callback = std::function<void(EntityID, EntityID)>;
    void update(EntityManager& m_entity_manager)
    {
        for (auto e : EntityCapture<Interactable>(m_entity_manager))
        {
            for (auto ee : EntityCapture<Controllable>(m_entity_manager))
            {
                auto pos1  = m_entity_manager.get<Position>(e);
                auto inte1 = m_entity_manager.get<Interactable>(e);
                auto pos2  = m_entity_manager.get<Position>(ee);
                // std::cout << " abs(pos1->x - pos2->x)" << abs(pos1->x - pos2->x)
                //           << " abs(pos1->y - pos2->y):" << abs(pos1->y - pos2->y)
                //           << "  <= inte1->radius_y):" << (inte1->radius_y) << "\n";
                if ((abs(pos1->x - pos2->x) <= inte1->radius_x) &&
                    (abs(pos1->y - pos2->y) <= inte1->radius_y))
                {
                    std::cout << "size:" << m_handlers.size() << "\n";
                    execute(e, ee);
                }
            }
        }
    }

    void listen(Callback cb)
    {
        std::cout << "Listening\n";
        m_handlers.push_back(cb);
        // std::cout << "size:" << m_handlers.size() << "\n";
    }

    void execute(EntityID e1, EntityID e2)
    {
        for (auto& handler : m_handlers)
        {
            std::cout << "CallingHandler\n";
            handler(e1, e2);
        }
    }

  public:
    std::vector<Callback> m_handlers;
};

class GameState : public State
{
  public:
    GameState(StateStack& stack, Context context)
        : State(stack, context)
        , m_shape(15)
        , m_rectangle_shape({10, 10})
        , m_controller()
        , m_vicinity_system()
    {
        m_controller.m_entity_manager = &m_entity_manager;
        m_controller.m_keyboard       = m_context.keyboard;
        m_shape.setFillColor(sf::Color::White);
        m_rectangle_shape.setFillColor(sf::Color::Red);

        auto e1 = m_entity_manager.create_entity();
        auto e2 = m_entity_manager.create_entity();
        m_entity_manager.attach<Controllable>(e1);
        m_entity_manager.attach<Position>(e1);

        *m_entity_manager.get<Position>(e1) = {2.5, 2.5};

        m_entity_manager.get<Position>(e1)->y = 200;
        m_entity_manager.attach<Interactable>(e2);
        m_entity_manager.attach<Position>(e2);
        m_controller.make_controller();
        m_controller.get_interactables();

        auto check_interactable = [=](EntityID e1, EntityID e2)
        {
            if (m_context.keyboard->is_key_pressed(sf::Keyboard::Key::E))
            {
            }
        };
        m_vicinity_system.listen(check_interactable);
    }

    void draw()
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
            m_rectangle_shape.setPosition({pos->x, pos->y});
            if (m_entity_manager.get<Interactable>(e)->is_switch)
            {
                m_rectangle_shape.setFillColor(sf::Color::Blue);
            }
            m_context.window->draw(m_rectangle_shape);
        }
    }
    bool update(sf::Time dt)
    {
        m_controller.update();
        m_vicinity_system.update(m_entity_manager);
        m_entity_manager.execute_actions();
    }
    bool handle_event(const sf::Event& event)
    {
        return true;
    }

  private:
    sf::RectangleShape m_rectangle_shape;
    sf::CircleShape    m_shape;
    Controller         m_controller;
    EntityManager      m_entity_manager;
    VicinitySystem     m_vicinity_system;
};

#endif /* GAME_STATE */