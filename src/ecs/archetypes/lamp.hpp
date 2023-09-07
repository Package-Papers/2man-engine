#pragma once
#ifndef TME_LAMP_ENTITY
#define TME_LAMP_ENTITY

#include "../entity_capture.hpp"
#include "entity_object.hpp"

class LampArchetype : public EntityObject
{
  public:
    LampArchetype(float x, float y)
    {
        pos_x = x;
        pos_y = y;
    }

    virtual EntityID create(EntityManager& em) override
    {
        auto lamp_entity = em.create_entity();
        em.attach<Position>(lamp_entity);
        em.attach<Interactable>(lamp_entity);
        em.attach<Colour>(lamp_entity);

        em.get<Interactable>(lamp_entity)->action = [](EntityManager* em, EntityID e1, EntityID e2)
        {
            auto col = em->get<Colour>(e1);
            col->r   = static_cast<uint8_t>(col->r + 1);
            col->g   = static_cast<uint8_t>(col->g + 1);
            col->b   = static_cast<uint8_t>(col->b + 1);
        };

        *em.get<Colour>(lamp_entity)   = {255, 255, 255};
        *em.get<Position>(lamp_entity) = {pos_x, pos_y};

        return lamp_entity;
    }

  private:
    float pos_x;
    float pos_y;
};

#endif /* TME_LAMP_ENTITY */
