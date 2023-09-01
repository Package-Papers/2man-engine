#pragma once
#ifndef MENU_STATE
#define MENU_STATE

#include <iostream>

#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "debug.hpp"

#include "state.hpp"

#include "settings.hpp"

enum OptionNames
{
    PLAY,
    EXIT,
};

class MenuState : public State
{
  public:
    MenuState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handle_event(const sf::Event& event);
    bool         empty()
    {
        return {m_options.size() == 0};
    }

};

void MenuState::update_option_text()
{
    if (m_options.empty())
        return;

    for ()
}

ImGui::Button play_option;
play_option.setFont(font);
play_option.setString("Play");
play_option.setPosition(context.window->getView().getSize() / 2.f);
m_options.push_back(play_option);

void MenuState::updateOptionText()
{
    if (ImGui::Button("play"))
    {
        request_stack_pop();
        request_stack_push(states::Game);
    }
    else if (ImGui::Button("end"))
    {
        request_stack_pop();
    }
        return;
}

#endif /* MENU_STATE */