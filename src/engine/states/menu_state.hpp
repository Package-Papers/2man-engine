#pragma once
#ifndef MENU_STATE
#define MENU_STATE

#include <imgui.h>

#include "engine/states/state.hpp"

enum OptionNames
{
    PLAY,
    EXIT,
};

class MenuState : public State
{
  public:
    MenuState(StateStack& stack, Context context)
        : State(stack, context)
    {
    }

    virtual void draw()
    {
    }
    virtual bool update(sf::Time dt)
    {
        this->update_option_text();
        return true;
    }
    virtual bool handle_event(const sf::Event& event)
    {
        return true;
    }

    void update_option_text();
};

inline void MenuState::update_option_text()
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
}

#endif /* MENU_STATE */