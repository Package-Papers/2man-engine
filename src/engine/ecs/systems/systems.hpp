#pragma once
#ifndef TME_SYSTEMS
#define TME_SYSTEMS


#include <memory>

#include "engine/ecs/systems/system_base.hpp"

class Systems
{
  public:
    /**
     * Retrieve system unique ID.
     */
    template <typename C>
    uint32_t get_system_id()
    {
        static uint32_t system_id = m_system_count++;
        return system_id;
    }

    /**
     * Add system to the systems pool.
     */
    template <typename System, typename... Args>
    System* add_system(Args&&... args)
    {
        auto system_id       = get_system_id<System>();
        m_systems[system_id] = std::make_unique<System>(std::forward<Args>(args)...);
        return static_cast<System*>(m_systems[system_id].get());
    }

    /**
     * Retrieve the system of specified type.
     */
    template <typename System>
    System* get_system()
    {
        auto system_id = get_system_id<System>();

        if (m_systems.contains(system_id))
            return static_cast<System*>(m_systems[system_id].get());

        return nullptr;
    }

    /**
     * Loop through all systems and update each.
     */
    void update(EntityManager& em, sf::Time dt)
    {
        for (auto& [_, system] : m_systems)
        {
            system->update(em, dt);
        }
    }

    /**
     * Draw all systems.
     */
    void draw(EntityManager& em)
    {
        for (auto& [_, system] : m_systems)
        {
            system->draw(em);
        }
    }

  private:
    std::unordered_map<uint32_t, std::unique_ptr<SystemBase>> m_systems;
    uint32_t                                                  m_system_count = 0;
};

#endif /* TME_SYSTEMS */