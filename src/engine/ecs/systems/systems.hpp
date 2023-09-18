#pragma once
#ifndef TME_SYSTEMS
#define TME_SYSTEMS

#include "engine/ecs/systems/system_base.hpp"

#include <memory>

class Systems
{
  public:
    template <typename C>
    uint32_t get_system_id()
    {
        static uint32_t system_id = g_system_count++;
        return system_id;
    }

    template <typename System, typename... Args>
    System* add_system(Args&&... args)
    {
        auto system_id       = get_system_id<System>();
        m_systems[system_id] = std::make_unique<System>(std::forward<Args>(args)...);
        return static_cast<System*>(m_systems[system_id].get());
    }

    template <typename System>
    System* get_system()
    {
        auto system_id = get_system_id<System>();

        if (m_systems.contains(system_id))
            return static_cast<System*>(m_systems[system_id].get());

        return nullptr;
    }

    void update(EntityManager& em)
    {
        for (auto& [_, system] : m_systems)
        {
            system->update(em);
        }
    }

    void draw(EntityManager& em)
    {
        for (auto& [_, system] : m_systems)
        {
            system->draw(em);
        }
    }

  private:
    std::unordered_map<uint32_t, std::unique_ptr<SystemBase>> m_systems;
    uint32_t                                                  g_system_count = 0;
};

#endif /* TME_SYSTEMS */