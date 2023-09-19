#pragma once
#ifndef TME_STATE_BASE
#define TME_STATE_BASE

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "engine/core/engine_context.hpp"
#include "engine/ecs/ecs.hpp"

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
    m_systems.update(m_entity_manager);
}

inline void State::draw_systems()
{
    m_systems.draw(m_entity_manager);
}

#endif /* TME_STATE */