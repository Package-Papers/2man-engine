#pragma once
#ifndef TITLE_STATE
#define TITLE_STATE

#include "states/state.hpp"

#include "controller.hpp"

#include "ecs/ecs.hpp"
#include "ecs/archetypes/button.hpp"
#include "ecs/systems/rendering_button.hpp"

class TitleState : public State
{
  public:
    TitleState(StateStack& stack, Context context)
        : State(stack, context)
        , m_rectangle_box({50, 50})

    {
        m_context.textures->load(textures::ID::Button1, "assets/graphics/buttons/menu-button.png");
        auto size = context.window->getSize();
        m_rectangle_box.setFillColor(sf::Color::White);
        auto [x, y] = m_context.window->getSize();
        ButtonArchetype button_factory(100.f, 100.f, textures::ID::Button1);
        auto            start_entity = button_factory.create_button(
                       m_entity_manager, static_cast<float>(x) / 2.f, static_cast<float>(y) / 2.f, "Start");
        auto end_entity = button_factory.create_button(
            m_entity_manager, static_cast<float>(x) / 8.f, static_cast<float>(y) / 8.f, "End");

        m_entity_manager.get<Button>(start_entity)->action = [=, this]()
        {
            request_stack_pop();
            request_stack_push(states::MainMenu);
        };

        m_entity_manager.get<Button>(end_entity)->action = [=, this]() { request_stack_pop(); };

        m_systems.add_system<RenderingButtonSystem>(m_entity_manager, m_context);
    }

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handle_event(const sf::Event& event);

  private:
    sf::RectangleShape m_rectangle_box;
    sf::Font           m_font;

    bool     m_show_text;
    sf::Time m_text_effect_time;
};

inline void TitleState::draw()
{
    // m_context.window->draw(m_rectangle_box);
    draw_systems();
}

inline bool TitleState::handle_event(const sf::Event& event)
{
    return true;
}

inline bool TitleState::update(sf::Time dt)
{
    m_text_effect_time += dt;

    if (m_text_effect_time >= sf::seconds(0.5f))
    {
        m_show_text        = !m_show_text;
        m_text_effect_time = sf::Time::Zero;
    }

    m_systems.update(m_entity_manager);

    return true;
}

#endif /* TITLE_STATE */