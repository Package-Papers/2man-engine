/*
 * This implementation is referenced from David Colson's wonderful blog.
 * Found at: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
 */

#pragma once
#ifndef TME_ECS_COMPONENT_POOL
#define TME_ECS_COMPONENT_POOL

#include <vector>

#include "doctest.h"

#include "entity.hpp"

/*
 * This is simply a memory pool for components.
 */

struct ComponentPool
{
    ComponentPool(std::size_t type_size)
    {
        m_type_size = type_size;
        m_data      = new char[type_size * MAX_ENTITIES];
    }

    ~ComponentPool()
    {
        delete[] m_data;
    }

    void* get(uint32_t index)
    {
        return m_data + (index * m_type_size);
    }

    uint32_t m_type_size;
    char*    m_data{nullptr};
};

using ComponentPools = std::vector<ComponentPool>;

#endif /* TME_ECS_COMPONENT_POOL */
