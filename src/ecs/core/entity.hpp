/*
 * This implementation is referenced from David Colson's wonderful blog.
 * Found at: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
 */

#pragma once
#ifndef TME_ECS_ENTITY
#define TME_ECS_ENTITY

#include <stdint.h>

#include "../../bitmask.hpp"
#include "component.hpp"

using EntityVersion               = uint32_t;
using EntityIndex                 = uint32_t;
using EntityID                    = uint64_t;
constexpr uint8_t  MAX_COMPONENTS = 64;
constexpr uint32_t MAX_ENTITIES   = 1024;
using ComponentMask               = tme::Bitmask<MAX_COMPONENTS>;

struct EntityInfo
{
    EntityID      m_id;
    ComponentMask m_mask;
};

// Creates a new entity entry with entity ID and entity version.
inline EntityID create_entity_id(EntityIndex entity_id, EntityVersion version)
{
    return (static_cast<EntityID>(entity_id) << 32) | (static_cast<EntityID>(version));
}

// Get upper 32 bits, where the index resides.
inline EntityIndex get_entity_index(EntityID entity_id)
{
    return (entity_id >> 32);
}

// Get lower 32 bits, where the version resides.
inline EntityVersion get_entity_version(EntityID entity_id)
{
    return static_cast<EntityVersion>(entity_id);
}

// Check if the entity has a valid index.
inline bool is_entry_valid(EntityID entity_id)
{
    return get_entity_index(entity_id) != EntityIndex(-1);
}

constexpr EntityIndex INVALID_ENTITY_INDEX = -1;
#define INVALID_ENTRY create_entity_id(EntityIndex(-1), 0)

#endif /* TME_ECS_ENTITY */
