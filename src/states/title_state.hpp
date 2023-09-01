#pragma once
#ifndef TITLE_STATE
#define TITLE_STATE

#include "../state.hpp"

class TitleState : public State
{
  public:
    TitleState(StateStack& stack, Context context)
        : State(stack, context)
        , m_rectangle_box({200, 200})
        , m_text(m_font)

    {
        auto _ = m_font.loadFromFile("/Users/nambouchara/2man-engine/font.ttf");
        m_text.setString("Hello World");
        auto         size     = context.window->getSize();
        sf::Vector2f size_rec = {static_cast<float>(size.x), static_cast<float>(size.y)};
        m_rectangle_box.setSize(size_rec);
        m_rectangle_box.setFillColor(sf::Color::White);
    }

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handle_event(const sf::Event& event);

  private:
    sf::RectangleShape m_rectangle_box;
    sf::Font           m_font;
    sf::Text           m_text;

    bool     m_show_text;
    sf::Time m_text_effect_time;
};

inline void TitleState::draw()
{
    m_context.window->draw(m_rectangle_box);
    m_context.window->draw(m_text);
}

inline bool TitleState::handle_event(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        request_stack_pop();
        request_stack_push(states::MainMenu);
    }
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
    return true;
}

#endif /* TITLE_STATE */