
#include "imgui.h"
#include "resources.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <unordered_map>

using resource_holder::FontHolder;
using resource_holder::TextureHolder;

namespace States
{
enum ID
{
    TitleScreen,
    MainMenu,
    Game,
    Pause
};
}

class StateStack : private sf::NonCopyable
{
  public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };
    template <typename T>
    void registerState(States::ID stateID);

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);
    void pushState(States::ID stateID);
    void popState();
    void clearStates();
    bool isEmpty() const;

  private:
    struct PendingChange
    {
        Action     action;
        States::ID stateID;
    };
    std::vector<State::Ptr>    mStack;
    std::vector<PendingChange> mPendingList;
    State::Context             mContext;
    // Given that we do not want to create all state objects from the beginning,
    // we can instead use factory functions that createa  new state on-demand.
    std::map<States::ID, std::function<State::Ptr()>> mFactories;

    // Fully written functions below.

    template <typename T>
    void StateStack::registerState(States::ID stateID)
    {
        mFactories[stateID] = [this]() { return State::Ptr(new T(*this, mContext)) }
    }

    State::Ptr StateStack::createState(States::ID stateID)
    {
        auto found = mFactories.find(stateID);
        assert(found != mFactories.end());

        return found->second();
    }
    void StateStack::handleEvent(const sf::Event& event)
    {
        for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
        {
            if (!(*itr)->handleEvent(event))
                return;
        }
        applyPendingChanges();
    }
    void StateStack::applyPendingChanges()
    {
        for (const PendingChange& change : mPendingList)
        {
            switch (change.action)
            {
                case Push:
                    mStack.push_back(createState(change.stateID));
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

class State
{
  public:
    typedef std::unique_ptr<State> Ptr;
    struct Context
    {
        Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);

        sf::RenderWindow* window;
        TextureHolder*    textures;
        FontHolder*       fonts;
    };

  public:
    State(StateStack& stack, Context context);
    virtual ~State()
        : virtual void draw()                        = 0;
    virtual bool update(sf::Time dt)                 = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;

  protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStateClear();

    Context getContext() const;
};

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
