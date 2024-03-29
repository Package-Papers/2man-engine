#pragma once
#ifndef TME_VICINITY_SYSTEM
#define TME_VICINITY_SYSTEM

#include "system_base.hpp"

struct VicinitySystem : public SystemBase
{
    using Callback = std::function<void(EntityManager*, EntityID, EntityID)>;

    virtual void update(EntityManager& m_entity_manager, sf::Time dt) override
    {
        for (auto e : EntityCapture<Interactable>(m_entity_manager))
        {
            for (auto ee : EntityCapture<Controllable>(m_entity_manager))
            {
                auto pos1  = m_entity_manager.get<Position>(e);
                auto inte1 = m_entity_manager.get<Interactable>(e);
                auto pos2  = m_entity_manager.get<Position>(ee);
                if ((abs(pos1->x - pos2->x) <= inte1->radius_x) &&
                    (abs(pos1->y - pos2->y) <= inte1->radius_y))
                {
                    execute(&m_entity_manager, e, ee);
                }
            }
        }
    }

    void listen(Callback cb)
    {
        m_handlers.push_back(cb);
    }

    void execute(EntityManager* m_entity_manager, EntityID e1, EntityID e2)
    {
        for (auto& handler : m_handlers)
        {
            handler(m_entity_manager, e1, e2);
        }
    }

  public:
    std::vector<Callback> m_handlers;
};

#endif /* TME_VICINITY_SYSTEM */