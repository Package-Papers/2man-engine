#pragma once
#ifndef TME_SYSTEM_BASE
#define TME_SYSTEM_BASE

#include <SFML/System/Time.hpp> 

#include "engine/ecs/entity_capture.hpp"
#include "engine/resource/resource_cache.hpp"

class EntityManager;

class SystemBase
{
  public:
    virtual void update(EntityManager& em, sf::Time dt) = 0;
    virtual void draw(EntityManager& em)
    {
    }
    virtual ~SystemBase()
    {
    }
};

#endif /* TME_SYSTEM_BASE */
