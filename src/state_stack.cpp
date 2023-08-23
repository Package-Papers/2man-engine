
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

class StateStack : private sf::NonCopyable
{
    public:
    enum Action
    {
    Push,
    Pop,
    Clear, 
    };

    private:
        // Given that we do not want to create all state objects from the beginning,
        // we can instead use factory functions that createa  new state on-demand.
        std::map<States::ID,std::function<State::Ptr()>> mFactories;

    template<typename T>
    void StateStack::registerState(States::ID stateID) {
        mFactories[stateID] = [this] () 
        {
            return State::Ptr(new T(*this, mContext))
        }
    }
    void State::Ptr StateStack::createState(states::ID stateID)
    {
        auto found = mFactories.find(stateID);
        assert(found != mFactories.end());

        return found->second;
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
       FOREACH(PendingChange change, mPendingList)
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
}

class State
{
    public:
        typedef std::unique_ptr<State> Ptr;
        struct Context { ... };

    public:
        virtual     State(State)

}