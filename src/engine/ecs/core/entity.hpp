/*
 * This implementation is referenced from David Colson's wonderful blog.
 * Found at: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
 */

#pragma once
#ifndef TME_ECS_ENTITY
#define TME_ECS_ENTITY

#include <stdint.h>

#include "engine/common/debug.hpp"
#include "engine/structures/bitmask.hpp"
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
inline bool is_entity_valid(EntityID entity_id)
{
    return get_entity_index(entity_id) != EntityIndex(-1);
}

constexpr EntityIndex INVALID_ENTITY_INDEX = -1;
#define INVALID_ENTITY create_entity_id(EntityIndex(-1), 0)

/*
 * Tests
 */

TEST_CASE("Creating a new entity ID")
{
    auto entity = create_entity_id(5, 2);

    CHECK((entity >> 32) == 5);
    CHECK(static_cast<uint32_t>(entity) == 2);

    auto entity_2 = create_entity_id(-1, 0xfe);
    CHECK(static_cast<uint32_t>(entity_2) == 0xfe);
    CHECK((entity_2 >> 32) == 0xffffffff);
}

TEST_CASE("Creatin an invalid entity")
{
    auto invalid_entity = INVALID_ENTITY;
    CHECK(!is_entity_valid(invalid_entity));

    auto entity_1 = create_entity_id(-2, 2);
    CHECK(is_entity_valid(entity_1));

    auto entity_2 = create_entity_id(2, EntityVersion(-1));
    CHECK(is_entity_valid(entity_2));
}

TEST_CASE("Checking entity version")
{

    auto entity_1 = create_entity_id(0, 5);
    CHECK(get_entity_version(entity_1) == 5);

    auto entity_2 = create_entity_id(0, 0xaa);
    CHECK(get_entity_version(entity_2) == 0xaa);

    auto entity_3 = create_entity_id(0, 0xfeed);
    CHECK(get_entity_version(entity_3) == 0xfeed);

    auto entity_4 = create_entity_id(0, EntityVersion(-1));
    CHECK(get_entity_version(entity_4) == EntityVersion(-1));
}

TEST_CASE("Checking entity index")
{
    auto entity_1 = create_entity_id(5, 0);
    CHECK(get_entity_index(entity_1) == 5);

    auto entity_2 = create_entity_id(0xaa, 0);
    CHECK(get_entity_index(entity_2) == 0xaa);

    auto entity_3 = create_entity_id(0xfeed, 0);
    CHECK(get_entity_index(entity_3) == 0xfeed);

    auto entity_4 = create_entity_id(INVALID_ENTITY_INDEX, 0);
    CHECK(get_entity_index(entity_4) == INVALID_ENTITY_INDEX);
}

#endif /* TME_ECS_ENTITY */
