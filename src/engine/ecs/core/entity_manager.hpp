/*
 * This implementation is referenced from David Colson's wonderful blog.
 * Found at: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
 */

#pragma once
#ifndef TME_ECS_ENTITY_MANAGER
#define TME_ECS_ENTITY_MANAGER

#include <functional>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "engine/common/debug.hpp"
#include "engine/ecs/action.hpp"
#include "engine/ecs/core/component.hpp"
#include "engine/ecs/core/component_pool.hpp"

/**
 * The EntityManager class acts as an interface for managing entities.
 *
 * The EntityManager class is reponsible for handing the Entities and
 * the Components (Systems excluded).
 *
 */
class EntityManager
{
  private:
    using EntityPool  = std::vector<EntityInfo>;
    using EntityQueue = std::vector<EntityIndex>;
    using ActionStack = std::vector<Action>;

  private:
    /** Attachs the specified `Component` to the entity.
     *
     * Returns the pointer to the component.
     *
     * @param entity_id the entity id to give the component to.
     */
    template <typename Component>
    Component* p_attach(EntityID entity_id);

    /** Retrieves the specified `Component` from the entity.
     *
     * Returns a pointer to the component if the entity has the component.
     * Returns nullptr if the entity does not have the component.
     *
     * @param entity_id the entity id to get the component from.
     */
    template <typename Component>
    Component* p_get(EntityID entity_id);

    /** Detachs the specified `Component` from the entity.
     *
     * @param entity_id the entity id to remove the component from.
     */
    template <typename Component>
    void p_detach(EntityID entity_id);

    template <std::size_t, typename... Components>
    struct ComponentRet
    {
        using RetType = std::tuple<Components*...>;

        // Base case
        template <typename Component>
        static std::tuple<Component*> make_tuple(bool attach, EntityManager& em, const EntityID e)
        {
            if (attach)
                return std::make_tuple(em.p_attach<Component>(e));
            else
                return std::make_tuple(em.p_get<Component>(e));
        }

        // Recursive case
        template <typename Component1, typename Component2, typename... Rest>
        static std::tuple<Component1*, Component2*, Rest*...>
            make_tuple(bool attach, EntityManager& em, const EntityID e)
        {
            return std::tuple_cat(
                make_tuple<Component1>(attach, em, e), // Form the head and attach it to the rest.
                make_tuple<Component2, Rest...>(attach, em, e));
        }

        static RetType attach(EntityManager& em, const EntityID e)
        {
            return make_tuple<Components...>(true, em, e);
        }

        static RetType get(EntityManager& em, const EntityID e)
        {
            return make_tuple<Components...>(false, em, e);
        }
    };

    template <typename... Components>
    struct ComponentRet<0, Components...>
    {
        using RetType = void;
        static RetType attach(EntityManager&, const EntityID)
        {
        }
        static RetType get(EntityManager&, const EntityID)
        {
        }
    };

    template <typename Component>
    struct ComponentRet<1, Component>
    {
        using RetType = Component*;
        static RetType attach(EntityManager& em, const EntityID e)
        {
            return em.p_attach<Component>(e);
        }
        static RetType get(EntityManager& em, const EntityID e)
        {
            return em.p_get<Component>(e);
        }
    };

  public:
    /** Create and return a new entity.
     *
     * If possible, this function will reuse
     * any entity id which has been marked for reuse.
     */
    EntityID create_entity();

    /** Destroy the entity.
     *
     * The entity index is invalidated and the version is incremented.
     * All components are also removed.
     *
     * @param entity_id the entity id to destroy.
     */
    void destroy_entity(EntityID entity_id);

    /** Attach specified component(s) to the entity.
     *
     * This function excepts 1 or more components to be specified in the template arguments.
     *
     * Example:
     *
     * EntityManager em;
     * auto entity = em.create_entity();
     * auto [pos, health] = em.attach<Position, Health>(entity);
     *
     * @param entity_id the entity id to attach component(s) to.
     *
     */
    template <typename... Components>
    typename ComponentRet<sizeof...(Components), Components...>::RetType
        attach(const EntityID entity_id)
    {
        return ComponentRet<sizeof...(Components), Components...>::attach(*this, entity_id);
    }

    /** Retrive specified component(s) from the entity.
     *
     * This function excepts 1 or more components to be specified in the template arguments.
     * If the component does not exist, nullptr is returned.
     *
     * Example:
     *
     * EntityManager em;
     * auto entity = em.create_entity();
     * em.attach<Position, Health>(entity);
     * auto [pos, health] = em.get<Position, Health>(entity);
     *
     * @param entity_id the entity id to retrieve component(s) from.
     */
    template <typename... Components>
    typename ComponentRet<sizeof...(Components), Components...>::RetType
        get(const EntityID entity_id)
    {
        return ComponentRet<sizeof...(Components), Components...>::get(*this, entity_id);
    }

    /** Remove specified component(s) from the entity.
     *
     * This function excepts 1 or more components to be specified in the template arguments.
     *
     * Example:
     *
     * EntityManager em;
     * auto entity = em.create_entity();
     * em.attach<Position, Health>(entity);
     * em.deatch<Position, Health>(entity);
     * auto [pos, health] = em.get<Position, Health>(entity); <--- [nullptr, nullptr]
     *
     * @param entity_id the entity id to detach component(s) from.
     */
    template <typename... Components>
    void detach(EntityID entity_id);

    template <typename Component>
    [[nodiscard]] bool        has_component(EntityID entity_id);
    [[nodiscard]] EntityPool& get_entities();

    /** Creates and queues an action onto the action stack.
     *
     * A pointer to the action which has been newly created is returned.
     */
    [[nodiscard]] Action* create_action();

    /** Execute all actions whichh as been queued.
     *
     * This function flushes the action stack.
     */
    void execute_actions();

  private:
    // Note: We want to minimize checking. Checking only happens in debug mode.
    ComponentPool& get_component_pool(std::size_t component_id);

    // Returns the memory location of the entity's specified component.
    void* get_entity_component_mem(EntityIndex entity_index, std::size_t component_id);

    // Returns true when the entity id is itself (valid).
    //
    // Implementation notes: Entity IDs are ALL unique. Each time an entity is deleted the version
    // number is incremented. So once an entity has been deleted it becomes invalid, even if we were
    // to reuse the slot, because we increment the version.
    bool entity_is_valid(EntityID entity_id);

  private:
    EntityPool m_entity_pool;

    // A list of entity indexes which can be reused for entity creation.
    EntityQueue m_free_entities;

    // A collection of component pools. Each pool holds the data of the component
    // for each entity, which can be accessed via the entity index.
    ComponentPools m_component_pools;

    // A collection of actions which has been requested. Can be
    // flushed and executed by calling `execute_actions`.
    ActionStack m_action_stack;
};

template <typename Component>
inline Component* EntityManager::p_attach(EntityID entity_id)
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
inline Component* EntityManager::p_get(EntityID entity_id)
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

inline EntityID EntityManager::create_entity()
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

template <typename Component>
inline void EntityManager::p_detach(EntityID entity_id)
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

template <typename... Components>
inline void EntityManager::detach(EntityID entity_id)
{
    (p_detach<Components>(entity_id), ...);
}

template <typename Component>
inline bool EntityManager::has_component(EntityID entity_id)
{
    // Check if entity is invalid.
    if (!entity_is_valid(entity_id))
        return false;

    auto entity_index = get_entity_index(entity_id);
    auto component_id = get_component_id<Component>();
    return m_entity_pool[entity_index].m_mask.get_bit(component_id);
}

#endif /* TME_ECS_ENTITY_MANAGER */
