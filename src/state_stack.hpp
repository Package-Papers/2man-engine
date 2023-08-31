#pragma once
#ifndef TME_STATE_STACK
#define TME_STATE_STACK

#include <iostream>

#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "debug.hpp"
#include "imgui.h"
#include "non_copyable.hpp"
#include "resources.hpp"
#include <map>
#include <unordered_map>

#include "settings.hpp"

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

class StateStack;

class State
{
  public:
    using Ptr           = std::unique_ptr<State>;
    using FontHolder    = resource_holder::FontHolder;
    using TextureHolder = resource_holder::TextureHolder;

    struct Context
    {
        Context()
        {
        }
        sf::RenderWindow* window;
        TextureHolder*    textures;
        FontHolder*       fonts;
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

  public:
    void request_stack_push(states::ID stateID);
    void request_stack_pop();
    void request_state_clear();

    Context get_context() const;

  private:
    StateStack* m_state_stack;
    Context     m_context;
};

class StateStack : private tme::NonCopyable
{
    friend State;

  public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };
    // template <typename T>
    // void registerState(states::ID stateID);
    // StateStack(const State::Context& ctx)
    // : m_context{ctx}
    // {

    // }

    void update(sf::Time dt);
    void draw();
    void push_state(State::Ptr state);
    void pop_state()
    {
        m_stack.pop_back();
    }
    void clear_states()
    {
        m_stack.clear();
    }
    bool is_empty() const
    {
        return (m_stack.size() == 0);
    }

    std::size_t size() noexcept
    {
        return m_stack.size();
    }
    std::size_t num_registered_states() noexcept
    {
        return m_factories.size();
    }
    std::size_t num_pending_states() noexcept
    {
        return m_pending_list.size();
    }

  private:
    struct PendingChange
    {
        Action     action;
        states::ID stateID;
    };
    std::vector<State::Ptr>    m_stack;
    std::vector<PendingChange> m_pending_list;
    State::Context             m_context;
    // Given that we do not want to create all state objects from the beginning,
    // we can instead use factory functions that createa  new state on-demand.
    std::map<states::ID, std::function<State::Ptr()>> m_factories;

  public:
    // Fully written functions below.
    template <typename T>
    void register_state(states::ID stateID)
    {
        m_factories[stateID] = [this]() { return State::Ptr(new T(*this, m_context)); };
    }

    State::Ptr create_state(states::ID stateID)
    {
        auto found = m_factories.find(stateID);
        assert(found != m_factories.end());

        return found->second();
    }
    void handle_event(const sf::Event& event)
    {
        for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
        {
            if (!(*itr)->handle_event(event))
                return;
        }
        apply_pending_changes();
    }
    void apply_pending_changes()
    {
        for (const PendingChange& change : m_pending_list)
        {
            switch (change.action)
            {
                case Push:
                    m_stack.push_back(create_state(change.stateID));
                    break;
                case Pop:
                    m_stack.pop_back();
                    break;
                case Clear:
                    m_stack.clear();
                    break;
            }
        }
        m_pending_list.clear();
    }
};

void State::request_stack_push(states::ID stateID)
{
    m_state_stack->m_pending_list.push_back({StateStack::Action::Push, stateID});
}
void StateStack::push_state(State::Ptr state)
{
    m_stack.push_back(std::move(state));
}

TEST_CASE("Test")
{
    auto stack = StateStack();
    class MenuState : public State
    {
      public:
        virtual void draw()
        {
        }
        virtual bool update(sf::Time dt)
        {
            return true;
        }
        virtual bool handle_event(const sf::Event& event)
        {
            return true;
        }
        MenuState(StateStack& stack, Context context)
            : State(stack, context)
        {
        }
    };
    stack.register_state<MenuState>(states::MainMenu);
    SUBCASE("check if the stack is empty after initialising")
    {
        CHECK(stack.size() == 0);
        CHECK(stack.is_empty());
    }
    SUBCASE("registering a State to a mFactories")
    {

        CHECK(stack.num_registered_states() != 0);
        stack.register_state<MenuState>(states::MainMenu);
        CHECK(stack.num_registered_states() == 1);
    }
    SUBCASE("Testing for createState function")
    {
        auto new_state = stack.create_state(states::MainMenu);
        auto tmp       = dynamic_cast<MenuState*>(new_state.get());
        CHECK(tmp != nullptr);
    }
    SUBCASE("Testing for apply_pending_changes")
    {
        auto new_state = stack.create_state(states::MainMenu);
        new_state->request_stack_push(states::MainMenu);
        CHECK(stack.num_pending_states() == 1);
        stack.apply_pending_changes();
        CHECK(stack.size() == 1);
    }
    SUBCASE("Testing for handle_events")
    {
    }
}

#endif /* TME_STATE_STACK */