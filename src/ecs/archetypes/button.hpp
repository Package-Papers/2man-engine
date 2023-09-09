#pragma once
#ifndef TME_BUTTON_ENTITY
#define TME_BUTTON_ENTITY

#include <SFML/Graphics.hpp>

#include "../entity_capture.hpp"
#include "entity_object.hpp"
#include "../systems/rendering_button.hpp"





class ButtonArchetype: public EntityObject
{
    public:

    ButtonArchetype(float x, float y)
    {
        size_x = x;
        size_y = y;
    }

    virtual EntityID create(EntityManager& em) override
    {
        auto button_entity = em.create_entity();
        em.attach<Position>(button_entity);
        auto button = em.attach<Button>(button_entity);
        button->size_x = size_x;
        button->size_y = size_y;

        return button_entity;

    }

    EntityID create_button(EntityManager& em, float pos_x, float pos_y, const std::string& text)
    {
        auto button_entity = create(em);
        *em.get<Position>(button_entity) = {pos_x, pos_y};
        em.get<Button>(button_entity)->text = {text};
        return button_entity;
    }

    private:
        float pos_x;
        float pos_y;
        float size_x;
        float size_y;
        
};

#endif /* TME_BUTTON_ENTITY */