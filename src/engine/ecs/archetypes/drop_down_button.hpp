#pragma once
#ifndef TME_DROP_DOWN_BUTTON_ENTITY
#define TME_DROP_DOWN_BUTTON_ENTITY

#include "button.hpp"

class DropDownButton : public ButtonArchetype
{ 

    public:
    DropDownButton(float x, float y, textures::ID texture): ButtonArchetype(x, y, texture), m_expanded(false) {}

    virtual EntityID create(EntityManager& em) override
    {
        auto button_entity = ButtonArchetype::create(em);
        return button_entity;
    }

    EntityID create_button(EntityManager& em, float pos_x, float pos_y, const std::string& text)
    {
        auto button_entity                  = create(em);
        *em.get<Position>(button_entity)    = {pos_x, pos_y};
        em.get<Button>(button_entity)->text = {text};
        return button_entity;
    }

    bool is_expanded()
    {
        return m_expanded;
    }

    void toggle_expanded()
    {
        m_expanded = !m_expanded;
        return;
    }

    const std::vector<ButtonArchetype>& get_child_buttons()
    {
        return m_child_buttons;
    }

    void push(const ButtonArchetype& child_button)
    {
        m_child_buttons.push_back(child_button);
    }


    private:
        std::vector<ButtonArchetype> m_child_buttons;
        bool m_expanded = false;
};

#endif /* TME_DROP_DOWN_BUTTON_ENTITY */