
#pragma once
#ifndef TME_ECS_ENTITY_MANAGER
#define TME_ECS_ENTITY_MANAGER

#include "entity.hpp"

class EntityManager
{
  private:
    using EntityPool = std::vector<EntityInfo>;

  public:
    EntityID create_entity()
    {
        EntityID entity_id = m_pool.size();
        m_pool.push_back({entity_id, ComponentMask()});
        return entity_id;
    }

    template <typename Component>
    void attach(EntityID id)
    {
        auto component_id = get_component_id<Component>();
        assert(component_id < MAX_COMPONENTS); // Prevent out of range.
        m_pool[id].m_mask.set_bit(component_id, true);
    }

    template <typename Component>
    void detach(EntityID id)
    {
        auto component_id = get_component_id<Component>();
        assert(component_id < MAX_COMPONENTS); // Prevent out of range.
        m_pool[id].m_mask.set_bit(component_id, false);
    }

  private:
    EntityPool m_pool;
};

#endif /* TME_ECS_ENTITY_MANAGER */
