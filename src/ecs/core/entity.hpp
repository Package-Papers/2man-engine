#pragma once
#ifndef TME_ECS_ENTITY
#define TME_ECS_ENTITY

#include <stdint.h>
#include <vector>

#include "../../bitmask.hpp"

using EntityID                   = uint64_t;
constexpr uint8_t MAX_COMPONENTS = 64;
using ComponentMask              = tme::Bitmask<MAX_COMPONENTS>;

struct EntityInfo
{
    EntityID      m_id;
    ComponentMask m_mask;
};

using EntityPool = std::vector<EntityInfo>;

inline EntityID create_entity(EntityPool& entity_pool)
{
    EntityID entity_id = entity_pool.size();
    entity_pool.push_back({entity_id, ComponentMask()});
    return entity_id;
}

#endif /* TME_ECS_ENTITY */
