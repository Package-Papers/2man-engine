#pragma once
#ifndef TME_PLAYER_ENTITY
#define TME_PLAYER_ENTITY

#include "engine/ecs/entity_capture.hpp"
#include "engine/ecs/components.hpp"
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
        em.attach<Controllable, Position, Velocity>(player_entity);

        *em.get<Position>(player_entity) = {pos_x, pos_y};
        *em.get<Velocity>(player_entity) = {0.f, 0.f, PLAYER_MAX_SPEED};

        return player_entity;
    }

  private:
    float pos_x;
    float pos_y;
    float PLAYER_MAX_SPEED = 80.f;
};

#endif /* TME_PLAYER_ENTITY */