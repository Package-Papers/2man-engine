/*
 * This implementation is referenced from David Colson's wonderful blog.
 * Found at: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
 */

#pragma once
#ifndef TME_ECS_ENTITY_MANAGER
#define TME_ECS_ENTITY_MANAGER

#include <iostream>
#include <stdexcept>
#include <vector>

#include "../../debug.hpp"

#include "component.hpp"
#include "component_pool.hpp"
#include "entity.hpp"

class EntityManager
{
  private:
    using EntityPool  = std::vector<EntityInfo>;
    using EntityQueue = std::vector<EntityIndex>;

  public:
    // Creates a new entity. Reuses an entry if available, else allocate a new slot.
    EntityID create_entity()
    {
        // If there is a free index we can reuse, we will take it.
        if (!m_free_entities.empty())
        {
            EntityIndex entity_index = m_free_entities.back();
            m_free_entities.pop_back();
            auto entity_version = get_entity_version(m_entity_pool[entity_index].m_id);

            // Validate the ID.
            EntityID entity_id               = create_entity_id(entity_index, entity_version);
            m_entity_pool[entity_index].m_id = entity_id;

            return entity_id;
        }

        // Allocate a new entry.
        // Note: Version starts at 0.
        EntityID entity_id = create_entity_id(m_entity_pool.size(), 0);

        if (get_entity_index(entity_id) >= MAX_ENTITIES)
        {
            throw std::runtime_error(
                "EntityManager::create_entity. Exceeded max entity, consider up scaling?");
        }

        m_entity_pool.push_back({entity_id, ComponentMask()});

        return entity_id;
    }

    // Note: Each time we destroy an entity, we invalidate the index and we increment the version.
    void destroy_entity(EntityID entity_id)
    {
        // Invalidate the index and increment the version.
        auto reuse_index                = get_entity_index(entity_id);
        auto new_version                = get_entity_version(entity_id) + 1;
        auto new_id                     = create_entity_id(INVALID_ENTITY_INDEX, new_version);
        m_entity_pool[reuse_index].m_id = new_id;

        // Reset the components bitmask for reusage.
        m_entity_pool[reuse_index].m_mask.clear();

        // Queue entry back for reuse.
        m_free_entities.push_back(reuse_index);
    }

    template <typename Component>
    Component* attach(EntityID entity_id)
    {
        // Entity deleted.
        if (!entity_is_valid(entity_id))
        {
            return nullptr;
        }

        auto component_id = get_component_id<Component>();
        assert(component_id < MAX_COMPONENTS); // Prevent out of range.

        // If the component pool is not large enough, add another entry for the current type.
        if (m_component_pools.size() <= component_id)
        {
            m_component_pools.resize(static_cast<std::size_t>(component_id + 1));
        }

        // If the component is new, we need to allocate a new pool for it.
        if (m_component_pools[component_id] == nullptr)
        {
            m_component_pools[component_id] = std::make_unique<ComponentPool>(sizeof(Component));
        }

        // Allocate the component data and attach it to the entity.
        auto       entity_index         = get_entity_index(entity_id);
        auto       component_memory_loc = get_entity_component_mem(entity_index, component_id);
        Component* component            = new (component_memory_loc) Component();

        // Mark component ownership.
        m_entity_pool[entity_index].m_mask.set_bit(component_id, true);

        return component;
    }

    template <typename Component>
    void detach(EntityID entity_id)
    {
        // Entity already deleted.
        if (!entity_is_valid(entity_id))
        {
            return;
        }

        auto component_id = get_component_id<Component>();
        assert(component_id < MAX_COMPONENTS); // Prevent out of range.
        m_entity_pool[get_entity_index(entity_id)].m_mask.set_bit(component_id, false);
    }

    template <typename Component>
    Component* get(EntityID entity_id)
    {
        // Entity already deleted.
        if (!entity_is_valid(entity_id))
        {
            return nullptr;
        }

        auto component_id = get_component_id<Component>();
        auto entity_index = get_entity_index(entity_id);

        // Check to make sure that the entity does have the component.
        if (!m_entity_pool[entity_index].m_mask.get_bit(component_id))
        {
            return nullptr;
        }

        Component* component =
            static_cast<Component*>(get_entity_component_mem(entity_index, component_id));
        return component;
    }

    template <typename Component>
    [[nodiscard]] bool has_component(EntityID entity_id)
    {
        // Check if entity is invalid.
        if (!entity_is_valid(entity_id)) return false;

        auto entity_index = get_entity_index(entity_id);
        auto component_id = get_component_id<Component>();
        return m_entity_pool[entity_index].m_mask.get_bit(component_id);
    }

    EntityPool& get_entities()
    {
        return m_entity_pool;
    }

  private:
    // Note: We want to minimize checking. Checking only happens in debug mode.
    ComponentPool& get_component_pool(std::size_t component_id)
    {
#ifndef NDEBUG
        if (component_id >= m_component_pools.size())
        {
            throw std::runtime_error("EntityManager::get_component, out of range");
        }
#endif
        return *m_component_pools[component_id];
    }

    // Returns the memory location of the entity's specified component.
    inline void* get_entity_component_mem(EntityIndex entity_index, std::size_t component_id)
    {
        // Entity has the component, return the memory address.
        ComponentPool& component_pool = get_component_pool(component_id);
        return component_pool.get(entity_index);
    }

    // Returns true when the entity id is itself (valid).
    //
    // Implementation notes: Entity IDs are ALL unique. Each time an entity is deleted the version
    // number is incremented. So once an entity has been deleted it becomes invalid, even if we were
    // to reuse the slot, because we increment the version.
    bool entity_is_valid(EntityID entity_id)
    {
        // Return true if version found matches the current one.
        return m_entity_pool[get_entity_index(entity_id)].m_id == entity_id;
    }

  private:
    EntityPool m_entity_pool;

    // A list of entity indexes which can be reused for entity creation.
    EntityQueue m_free_entities;

    // A collection of component pools. Each pool holds the data of the component for each entity,
    // which can be accessed via the entity index.
    ComponentPools m_component_pools;
};

#endif /* TME_ECS_ENTITY_MANAGER */
