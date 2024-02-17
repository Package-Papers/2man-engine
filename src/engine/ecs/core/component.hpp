/*
 * This implementation is referenced from David Colson's wonderful blog.
 * Found at: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
 */

#pragma once
#ifndef TME_ECS_COMPONENT
#define TME_ECS_COMPONENT

#include "engine/common/debug.hpp"

#include <cstdint>

extern uint32_t g_component_count;

// Return a unique id assigned to a given component type.
template <typename C>
uint32_t get_component_id()
{
    // Each template specialization invokes an ID increment.
    // Subsequent calls by the same type will yield the ID without incrementing.
    static uint32_t component_id = g_component_count++;
    return component_id;
}

#endif /* TME_ECS_COMPONENT */
