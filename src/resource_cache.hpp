#pragma once
#ifndef TME_RESOURCE_CACHE
#define TME_RESOURCE_CACHE

#include "resources.hpp"

template <typename Resource, typename Identifier>
struct ResourceCache
{
  public:
    ResourceCache(resource_holder::ResourceHolder<Resource, Identifier>* holder)
        : m_resource_holder(holder)
    {
    }

    Resource* load(Identifier id)
    {
        if (m_resource_cache.contains(id))
        {
            return m_resource_cache[id];
        }
        else
        {
            auto& resource = m_resource_holder->get(id);
            m_resource_cache[id] = &resource;
            return &resource;
        }
    }

    std::unordered_map<Identifier, Resource*>              m_resource_cache;
    resource_holder::ResourceHolder<Resource, Identifier>* m_resource_holder;
};

#endif /* TME_RESOURCE_CACHE */
