#pragma once
#ifndef TME_VICINITY_SYSTEM
#define TME_VICINITY_SYSTEM

#include "system_base.hpp"

struct VicinitySystem : public SystemBase
{
    using Callback = std::function<void(EntityManager*, EntityID, EntityID)>;

    virtual void update(EntityManager& m_entity_manager) override
    {
        for (auto e : EntityCapture<Interactable>(m_entity_manager))
        {
            for (auto ee : EntityCapture<Controllable>(m_entity_manager))
            {
                auto pos1  = m_entity_manager.get<Position>(e);
                auto inte1 = m_entity_manager.get<Interactable>(e);
                auto pos2  = m_entity_manager.get<Position>(ee);
                // std::cout << " abs(pos1->x - pos2->x)" << abs(pos1->x - pos2->x)
                //           << " abs(pos1->y - pos2->y):" << abs(pos1->y - pos2->y)
                //           << "  <= inte1->radius_y):" << (inte1->radius_y) << "\n";
                if ((abs(pos1->x - pos2->x) <= inte1->radius_x) &&
                    (abs(pos1->y - pos2->y) <= inte1->radius_y))
                {
                    // std::cout << "size:" << m_handlers.size() << "\n";
                    execute(&m_entity_manager, e, ee);
                }
            }
        }
    }

    void listen(Callback cb)
    {
        // std::cout << "Listening\n";
        m_handlers.push_back(cb);
        // std::cout << "size:" << m_handlers.size() << "\n";
    }

    void execute(EntityManager *m_entity_manager, EntityID e1, EntityID e2)
    {
        for (auto& handler : m_handlers)
        {
            // std::cout << "CallingHandler\n";
            handler(m_entity_manager, e1, e2);
            // std::cout << "CallingHandler2\n";
        }
    }

  public:
    std::vector<Callback> m_handlers;
};

#endif /* TME_VICINITY_SYSTEM */