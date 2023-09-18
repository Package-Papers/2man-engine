#pragma once
#ifndef TME_SYSTEM_BASE
#define TME_SYSTEM_BASE

#include "engine/resource/resource_cache.hpp"
#include "engine/ecs/entity_capture.hpp"

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

#endif /* TME_SYSTEM_BASE */
