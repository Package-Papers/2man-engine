#pragma once
#ifndef TME_ACTION
#define TME_ACTION

#include <vector>
#include <functional>

#include "ecs/core/entity.hpp"

class EntityManager;

struct Action
{
    std::vector<EntityID> targets;
    std::function<void(EntityManager* em, EntityID)> func;
    
    void execute(EntityManager* em)
    {
        for (auto e : targets)
        {
            func(em, e);
        }
    }
};

#endif /* TME_ACTION */
