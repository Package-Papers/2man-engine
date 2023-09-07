#pragma once
#ifndef TME_SYSTEM_BASE
#define TME_SYSTEM_BASE

#include <memory>

#include "../entity_capture.hpp"

class EntityManager;

class SystemBase
{
  public:
    virtual void update(EntityManager& m_entity_manager) = 0;

    virtual ~SystemBase()
    {
    }
};

using Systems = std::vector<std::unique_ptr<SystemBase>>;

#endif /* TME_SYSTEM_BASE */
