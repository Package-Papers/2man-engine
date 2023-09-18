#pragma once
#ifndef TME_ENTITY_OBJECT
#define TME_ENTITY_OBJECT

#include "../entity_capture.hpp"

class EntityObject
{
  public:
    virtual EntityID create(EntityManager& em) = 0;
    auto             make()
    {
    }
};

#endif /* TME_ENTITY_OBJECT */
