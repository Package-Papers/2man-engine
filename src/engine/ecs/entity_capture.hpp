/*
 * This implementation is referenced from David Colson's wonderful blog.
 * Found at: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
 */

#pragma once
#ifndef TME_ECS_ENTITY_CAPTURE
#define TME_ECS_ENTITY_CAPTURE

#include <cstdlib>

#include "engine/common/debug.hpp"

#include "engine/ecs/core/component.hpp"
#include "engine/ecs/core/entity.hpp"
#include "engine/ecs/core/entity_manager.hpp"

/**
 * The EntityCapture returns an interable which contains all entities which satisfies the component query.
 */
template <typename... Components>
struct EntityCapture
{
    EntityCapture(EntityManager& entity_manager)
        : m_entity_manager(&entity_manager)
    {
        if (sizeof...(Components) == 0)
        {
            // Capture all entities
            m_all = true;
        }
        else
        {
            std::vector<uint32_t> component_ids;
            (component_ids.push_back(get_component_id<Components>()), ...);

            for (int i = 0; i < sizeof...(Components); i++)
            {
                m_component_mask.set_bit(component_ids[i], true);
            }
        }
    }

    struct Iterator
    {
        Iterator(EntityManager* entity_manager, EntityIndex entity_index, const ComponentMask* mask,
                 bool all)
            : m_entity_manager(entity_manager)
            , m_entity_index(entity_index)
            , m_mask(mask)
            , m_all(all)
        {
        }

        EntityID operator*() const
        {
            return m_entity_manager->get_entities()[m_entity_index].m_id;
        }

        // Returns whether the current index is valid.
        //
        // Note: If `m_all` is true, all indexes will be valid.
        //       Otherwise, we have to check whether the current entity at the index satisfies the
        //       component mask requirement.
        bool valid_index()
        {
            return (m_all ||
                    m_entity_manager->get_entities()[m_entity_index].m_mask.contains(*m_mask));
        }

        bool operator==(const Iterator& other) const
        {
            return m_entity_index == other.m_entity_index ||
                   m_entity_index == m_entity_manager->get_entities().size();
        }

        bool operator!=(const Iterator& other) const
        {
            return m_entity_index != other.m_entity_index &&
                   m_entity_index != m_entity_manager->get_entities().size();
        }

        // Returns the next valid entry.
        //
        // Note: The very last element this will return will be end(), m_entity_index ==
        // entities_size.
        Iterator& operator++()
        {
            // Note: This checks for two things.
            //   1. The index is still in range.
            //   2. The index is "valid". (Look at valid_index impl for more details)
            auto entities_size = m_entity_manager->get_entities().size();
            do
            {
                m_entity_index++;
            } while (m_entity_index < entities_size &&
                     !valid_index()); // If index is invalid, we skip over it.

            return *this;
        }

        EntityManager*       m_entity_manager;
        EntityIndex          m_entity_index;
        const ComponentMask* m_mask;
        bool                 m_all;
    };

    // Get the first valid iterator.
    const Iterator begin() const
    {
        uint32_t    first_index   = 0;
        auto&       entities      = m_entity_manager->get_entities();
        std::size_t entities_size = entities.size();

#define IN_RANGE           (first_index < entities_size)
#define NOT_CONTAIN        (!entities[first_index].m_mask.contains(m_component_mask))
#define GOT_INVALID_ENTITY (!is_entity_valid(entities[first_index].m_id))

        // We skip the entity if
        //  1. It doesn't satisfy the component mask.
        //  2. It is invalid (removed).
        while (IN_RANGE && (NOT_CONTAIN || GOT_INVALID_ENTITY))
        {
            first_index++;
        }

#undef IN_RANGE
#undef NOT_CONTAIN
#undef GOT_INVALID_ENTITY

        return Iterator(m_entity_manager, first_index, &m_component_mask, m_all);
    }

    // Returns the last element.
    const Iterator end() const
    {
        auto last_index = EntityIndex(m_entity_manager->get_entities().size());
        return Iterator(m_entity_manager, last_index, &m_component_mask, m_all);
    }

    bool           m_all = false;
    EntityManager* m_entity_manager;
    ComponentMask  m_component_mask;
};

#endif /* TME_ECS_ENTITY_CAPTURE */
