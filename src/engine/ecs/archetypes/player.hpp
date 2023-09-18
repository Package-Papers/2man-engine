#pragma once
#ifndef TME_PLAYER_ENTITY
#define TME_PLAYER_ENTITY

#include "../entity_capture.hpp"
#include "entity_object.hpp"

class PlayerArchetype : public EntityObject
{

  public:
    PlayerArchetype(float x, float y)
    {
        pos_x = x;
        pos_y = y;
    }

    virtual EntityID create(EntityManager& em) override
    {
        auto player_entity = em.create_entity();
        em.attach<Controllable>(player_entity);
        em.attach<Position>(player_entity);
        *em.get<Position>(player_entity) = {pos_x, pos_y};

        return player_entity;
    }

  private:
    float pos_x;
    float pos_y;
};

#endif /* TME_PLAYER_ENTITY */