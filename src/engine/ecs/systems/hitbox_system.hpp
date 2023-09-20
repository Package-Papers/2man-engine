#pragma once
#ifndef TME_HITBOX_SYSTEM
#define TME_HITBOX_SYSTEM

#include <set>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "engine/core/engine_context.hpp"
#include "engine/ecs/components.hpp"
#include "engine/ecs/core/entity.hpp"
#include "engine/ecs/entity_capture.hpp"
#include "engine/ecs/systems/system_base.hpp"

// Note(Appy): At the moment the hitbox system only check against other entities which also has the
// component.
//             In the future a collision group can be implmented.
//             Optimization of checking the hitbox should also be implemented.

class HitboxSystem : public SystemBase
{
  private:
    std::map<EntityID, std::vector<EntityID>> m_collision_events;

    // The actual hitbox which will be used to compare collision.
    sf::RectangleShape m_hitbox;
    sf::FloatRect      m_box_1, m_box_2;
    Context            m_context;

  public:
    HitboxSystem(Context context)
        : m_context(context)
    {
        m_hitbox.setFillColor(sf::Color::Transparent);
        m_hitbox.setOutlineColor(sf::Color::Green);
        m_hitbox.setOutlineThickness(1.f);
    }

    std::set<EntityID> get_entity_collision(EntityManager& em, EntityID e)
    {
        std::set<EntityID> participants{};
        auto hitbox_info_1 = em.get<Hitbox>(e);

        // If the hitbox is inactive, we skip.
        if (!hitbox_info_1->active)
            return{};

        auto pos_1 = em.get<Position>(e);

        set_hitbox(m_box_1, hitbox_info_1, pos_1);

        for (auto e1 : EntityCapture<Hitbox>(em))
        {
            // Avoid self collision.
            if (e != e1)
            {
                auto hitbox_info_2 = em.get<Hitbox>(e1);
                auto pos_2         = em.get<Position>(e1);

                if (!hitbox_info_2->active)
                    continue;

                set_hitbox(m_box_2, hitbox_info_2, pos_2);

                if (m_box_2.findIntersection(m_box_1))
                {
                    participants.insert(e1);
                }
            }
        }
        return participants;
    }

    void update(EntityManager& m_entity_manager, sf::Time dt) override
    {
        m_collision_events.clear();

        // Note: This can be optimized later.
        for (auto e1 : EntityCapture<Hitbox>(m_entity_manager))
        {
            auto hitbox_info_1 = m_entity_manager.get<Hitbox>(e1);

            // If the hitbox is inactive, we skip.
            if (!hitbox_info_1->active)
                continue;
            auto pos_1 = m_entity_manager.get<Position>(e1);

            set_hitbox(m_box_1, hitbox_info_1, pos_1);

            for (auto e2 : EntityCapture<Hitbox>(m_entity_manager))
            {
                // Avoid self collision.
                if (e1 != e2)
                {
                    auto hitbox_info_2 = m_entity_manager.get<Hitbox>(e2);
                    auto pos_2         = m_entity_manager.get<Position>(e2);

                    if (!hitbox_info_2->active)
                        continue;

                    set_hitbox(m_box_2, hitbox_info_2, pos_2);

                    if (m_box_2.findIntersection(m_box_1))
                    {
                        m_collision_events[e1].push_back(e2);
                        m_collision_events[e2].push_back(e1);
                    }
                }
            }
        }
    }

    void set_hitbox(sf::FloatRect& hb, Hitbox* info, Position* position)
    {
        hb.height = info->height;
        hb.width  = info->width;
        hb.left   = position->x + info->x_offset;
        hb.top    = position->y + info->y_offset;
    }

    void draw(EntityManager& m_entity_manager) override
    {
        for (auto e : EntityCapture<Hitbox>(m_entity_manager))
        {
            auto [hitbox_info, pos] = m_entity_manager.get<Hitbox, Position>(e);
            m_hitbox.setPosition({pos->x + hitbox_info->x_offset, pos->y + hitbox_info->y_offset});

            m_hitbox.setSize({hitbox_info->width, hitbox_info->height});

            m_context.window->draw(m_hitbox);
        }
    }

    [[nodiscard]] std::optional<std::vector<EntityID>> get_collision(EntityID e) 
    {
        if (m_collision_events.contains(e))
        {
            return m_collision_events[e];
        }
        return {};
    }

    void flush(EntityID e)
    {
        m_collision_events.erase(e);
    }

    bool is_entity_colliding(EntityManager& em, EntityID e)
    {
        std::set<EntityID> participants{};
        auto hitbox_info_1 = em.get<Hitbox>(e);

        // If the hitbox is inactive, we skip.
        if (!hitbox_info_1->active)
            return false;

        auto pos_1 = em.get<Position>(e);

        set_hitbox(m_box_1, hitbox_info_1, pos_1);

        for (auto e1 : EntityCapture<Hitbox>(em))
        {
            // Avoid self collision.
            if (e != e1)
            {
                auto hitbox_info_2 = em.get<Hitbox>(e1);
                auto pos_2         = em.get<Position>(e1);

                if (!hitbox_info_2->active)
                    continue;

                set_hitbox(m_box_2, hitbox_info_2, pos_2);

                if (m_box_2.findIntersection(m_box_1))
                {
                    return true;
                }
            }
        }
        return false;
    }
};

#endif /* TME_HITBOX_SYSTEM */
