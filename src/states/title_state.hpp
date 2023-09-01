#pragma once
#ifndef TITLE_STATE
#define TITLE_STATE

#include <iostream>

#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "debug.hpp"

#include "state.hpp"

#include "settings.hpp"

class TitleState : public State
{
  public:
    TitleState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handle_event(const sf::Event& event);

  private:
    sf::Sprite m_background_sprite;
    sf::Text   m_text;

    bool     m_show_text;
    sf::Time m_text_effect_time;
};

bool TitleState::handle_event(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        request_stack_pop();
        request_stack_push(states::MainMenu);
    }
    return true;
}

bool TitleState::update(sf::Time dt)
{
    m_text_effect_time += dt;

    if (m_text_effect_time >= sf::seconds(0.5f))
    {
        m_show_text        = !m_show_text;
        m_text_effect_time = sf::Time::Zero;
    }
    return true;
}

#endif /* TITLE_STATE */