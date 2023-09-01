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

class State;

class StateStack : private tme::NonCopyable
{
    friend State;

  public:
    StateStack() = default;

    void set_context(State::Context context)
    {
        this->m_context = context;
    }

  public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };

    void update(sf::Time dt)
    {
        if (this->is_empty())
            return;
    }
    void draw()
    {
        if (this->is_empty())
            return;
        this->top()->draw();
    }
    void push_state(State::Ptr);
    void pop_state()
    {
        m_stack.pop_back();
    }
    void clear_states()
    {
        m_stack.clear();
    }
    State* top()
    {
        return m_stack.back().get();
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
        if (m_stack.empty())
            return;
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

inline void StateStack::push_state(State::Ptr state)
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

    SUBCASE("check if the stack is empty after initialising")
    {
        CHECK(stack.size() == 0);
        CHECK(stack.is_empty());
    }
    SUBCASE("registering a State to a mFactories")
    {
        CHECK(stack.num_registered_states() == 0);
        stack.register_state<MenuState>(states::MainMenu);
        CHECK(stack.num_registered_states() == 1);
    }
    stack.register_state<MenuState>(states::MainMenu);
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

    auto new_state = stack.create_state(states::MainMenu);
    class TitleState : public State
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
        TitleState(StateStack& stack, Context context)
            : State(stack, context)
        {
        }
    };
    stack.register_state<TitleState>(states::TitleScreen);
    auto new_state2 = stack.create_state(states::TitleScreen);
    SUBCASE("Testing for apply_pending_changes complex")
    {
        new_state->request_stack_push(states::MainMenu);
        new_state2->request_stack_push(states::TitleScreen);
        CHECK(stack.num_pending_states() == 2);
        new_state2->request_stack_pop();
        CHECK(stack.num_pending_states() == 1);
        new_state->request_stack_pop();
        CHECK(stack.num_pending_states() == 0);
    }
    SUBCASE("Testing for clearing function works after apply_pending_changes")
    {
        new_state->request_stack_push(states::MainMenu);
        new_state2->request_stack_push(states::TitleScreen);
        stack.apply_pending_changes();
        CHECK(stack.num_pending_states() == 0);
    }
    SUBCASE("Testing for clearing function works after apply_pending_changes")
    {
        new_state->request_stack_push(states::MainMenu);
        new_state2->request_stack_push(states::TitleScreen);
        stack.apply_pending_changes();
        CHECK(stack.num_pending_states() == 0);
    }
}

#endif /* TME_STATE_STACK */