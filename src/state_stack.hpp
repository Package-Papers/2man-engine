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
        Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);

        sf::RenderWindow* window;
        TextureHolder*    textures;
        FontHolder*       fonts;
    };

  public:
    State(StateStack& stack, Context context);
    virtual ~State();
    virtual void draw()                               = 0;
    virtual bool update(sf::Time dt)                  = 0;
    virtual bool handle_event(const sf::Event& event) = 0;

  protected:
    void request_stack_push(states::ID stateID);
    void request_stack_pop();
    void request_state_clear();

    Context get_context() const;
};

class StateStack : private tme::NonCopyable
{
  public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };
    // template <typename T>
    // void registerState(states::ID stateID);

    void update(sf::Time dt);
    void draw();
    void push_state(states::ID stateID);
    void pop_state();
    void clear_states();
    bool is_empty() const;

  private:
    struct PendingChange
    {
        Action     action;
        states::ID stateID;
    };
    std::vector<State::Ptr>    mStack;
    std::vector<PendingChange> mPendingList;
    State::Context             mContext;

    // Given that we do not want to create all state objects from the beginning,
    // we can instead use factory functions that createa  new state on-demand.
    std::map<states::ID, std::function<State::Ptr()>> mFactories;

    // Fully written functions below.

    template <typename T>
    void register_state(states::ID stateID)
    {
        mFactories[stateID] = [this]() { return State::Ptr(new T(*this, mContext)); };
    }

    State::Ptr create_state(states::ID stateID)
    {
        auto found = mFactories.find(stateID);
        assert(found != mFactories.end());

        return found->second();
    }
    void handle_event(const sf::Event& event)
    {
        for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
        {
            if (!(*itr)->handle_event(event))
                return;
        }
        apply_pending_changes();
    }
    void apply_pending_changes()
    {
        for (const PendingChange& change : mPendingList)
        {
            switch (change.action)
            {
                case Push:
                    mStack.push_back(create_state(change.stateID));
                    break;
                case Pop:
                    mStack.pop_back();
                    break;
                case Clear:
                    mStack.clear();
                    break;
            }
        }
        mPendingList.clear();
    }
};

TEST_CASE("Statestack Initialisation")
{
    const tme::WindowContext default_ctx = {
        TME_WINDOW_WIDTH,
        TME_WINDOW_HEIGHT,
        TME_WINDOW_TITLE,
        TME_WINDOW_FRAMES_LIM,
        int_to_bool(TME_VERTICAL_SYNC),
        int_to_bool(TME_WINDOW_FULLSCREEN),
    };

    auto engine = tme::Engine(default_ctx);
    CHECK(true);
}

TEST_CASE("Stack Initialisation")
{
}

// In process.

// class Application
// {
//     public:
//         void Application::registerStates()
//         {
//             mStateStack.registerState<TitleState>(States::TitleScreen);
//             mStateStack.registerState<MenuState>(States::MainMenu);
//             mStateStack.registerState<GameState>(States::Game);
//             mStateStack.registerState<PauseState>(States::Pause);
//         }
//         void Application::processInput()
//         {
//             while (mWindow.pollEvent(event)) {
//                 mStateStack.handleEvent(event);
//             }
//         }
//         void Application::update(sf::Time dt)
//        {
//            mStateStack.update(dt);
//        }

// }

#endif /* TME_STATE_STACK */