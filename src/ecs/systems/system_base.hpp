#pragma once
#ifndef TME_SYSTEM_BASE
#define TME_SYSTEM_BASE

#include <memory>

#include "../../resources.hpp"
#include "../../state.hpp"
#include "../entity_capture.hpp"

class EntityManager;

class SystemBase
{
  public:
    virtual void update(EntityManager& m_entity_manager) = 0;
    virtual void draw(EntityManager& m_entity_manager)
    {
    }

    virtual ~SystemBase()
    {
    }
};

template <typename Resource, typename Identifier>
class ResourceCache
{
  public:
    ResourceCache(resource_holder::ResourceHolder<Resource, Identifier>* holder)
        : m_resource_holder(holder)
    {
    }

    std::unordered_map<Identifier, Resource*>              m_resource_cache;
    resource_holder::ResourceHolder<Resource, Identifier>* m_resource_holder;

    Resource* load(Identifier id)
    {
        if (m_resource_cache.contains(id))
        {
            return m_resource_cache[id];
        }
        else
        {
            std::cout << "Yes\n";
            if (m_resource_holder == nullptr)
                std::cout << "NULL!\n";
            auto& resource = m_resource_holder->get(id);
            std::cout << "Yes2\n";
            m_resource_cache[id] = &resource;
            std::cout << "Yes2\n";
            return &resource;
        }
    }
};

#endif /* TME_SYSTEM_BASE */
