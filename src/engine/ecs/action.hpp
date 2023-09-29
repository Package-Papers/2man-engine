#pragma once
#ifndef TME_ACTION
#define TME_ACTION

#include <functional>
#include <vector>

#include "engine/ecs/core/entity.hpp"

class EntityManager;

/**
 * @brief This object encapsulates a function which runs on the ECS world.
 *
 * This object is mainly used by the EntityManager. Actions are queued into the
 * a stack which is then flushed at the end of each frame.
 */
struct Action
{
    /**
     * @brief Executes the action on all entities specified.
     */
    void execute(EntityManager* const em)
    {
        for (const auto& e : targets)
        {
            func(em, e);
        }
    }

    /**
     * Target entities the action acts upon.
     */
    std::vector<EntityID>                            targets;

    /**
     * The action to be executed by the ECS entity manager.
     */ 
    std::function<void(EntityManager* const em, EntityID)> func; 
};

#endif /* TME_ACTION */
