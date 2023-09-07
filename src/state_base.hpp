#pragma once
#ifndef TME_STATE_BASE
#define TME_STATE_BASE

#include <iostream>

#include <SFML/Graphics.hpp>

#include "debug.hpp"
#include "ecs/systems/system_base.hpp"
#include "keyboard.hpp"
#include "resources.hpp"

class StateStack;

namespace states
{
enum ID
{
    TitleScreen,
    MainMenu,
    Game,
    Pause
};
}

class State
{
  public:
    using Ptr           = std::unique_ptr<State>;
    using FontHolder    = resource_holder::FontHolder;
    using TextureHolder = resource_holder::TextureHolder;

    struct Context
    {
        sf::RenderWindow* window;
        TextureHolder*    textures;
        FontHolder*       fonts;
        tme::Keyboard*    keyboard;
    };

  public:
    State(StateStack& stack, Context context)
        : m_state_stack(&stack)
        , m_context(context)
    {
    }
    virtual ~State()
    {
    }
    virtual void draw()                               = 0;
    virtual bool update(sf::Time dt)                  = 0;
    virtual bool handle_event(const sf::Event& event) = 0;
    void         update_systems();
    void         draw_systems();

  public:
    void request_stack_push(states::ID stateID);
    void request_stack_pop();
    void request_stack_clear();

    Context get_context() const;

  protected:
    StateStack*   m_state_stack;
    EntityManager m_entity_manager;
    Systems       m_systems;
    Context       m_context;
};

#include "state_stack.hpp"

inline void State::request_stack_push(states::ID stateID)
{
    m_state_stack->m_pending_list.push_back({StateStack::Action::Push, stateID});
}
inline void State::request_stack_pop()
{
    m_state_stack->m_pending_list.push_back({StateStack::Action::Pop});
}
inline void State::request_stack_clear()
{
    m_state_stack->m_pending_list.push_back({StateStack::Action::Clear});
}

inline void State::update_systems()
{
    for (auto& system : m_systems)
    {
        system->update(m_entity_manager);
    }
}

inline void State::draw_systems()
{
    for (auto& system : m_systems)
    {
        system->draw(m_entity_manager);
    }
}

#endif /* TME_STATE */