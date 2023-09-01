#pragma once
#ifndef GAME_STATE
#define GAME_STATE

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../common.hpp"
#include "../debug.hpp"

#include "../state.hpp"

#include "../settings.hpp"

class GameState : public State
{
  public:
    GameState(StateStack& stack, Context context)
        : State(stack, context)
    {
    }

    void draw()
    {
    }
    bool update(sf::Time dt)
    {
        return true;
    }
    bool handle_event(const sf::Event& event)
    {
        return true;
    }
};

#endif /* GAME_STATE */