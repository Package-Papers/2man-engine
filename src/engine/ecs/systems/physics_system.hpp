#pragma once
#ifndef TME_PHYSICS_SYSTEM
#define TME_PHYSICS_SYSTEM

#include "engine/utility/math.hpp"
#include "engine/ecs/components.hpp"
#include "engine/ecs/entity_capture.hpp"
#include "engine/ecs/systems/system_base.hpp"
#include "engine/ecs/systems/hitbox_system.hpp"


class PhysicsSystem : public SystemBase
{
  public:
  explicit PhysicsSystem(HitboxSystem* hs = nullptr)
  : m_hs(hs)
  {
  }

  void update(EntityManager &em, sf::Time dt) override
  {
    for (auto e : EntityCapture<Velocity>(em))
    {
      auto [vel, pos] = em.get<Velocity, Position>(e);  
      apply_friction(vel, m_DECELERATION);
      auto pos_snapshot = *pos;
      
      // Get the norm of the movement vector.
      auto norm = tme::normalize(vel->x, vel->y);
      norm.x = fabs(norm.x);
      norm.y = fabs(norm.y);

      apply_y_movement(norm.y, vel->y, pos, dt);
      // If collided, we move back to last position.
      if (m_hs->is_entity_colliding(em, e))
      {
        pos->y = pos_snapshot.y;
      }


      apply_x_movement(norm.x, vel->x, pos, dt);
      // If collided, we move back to last position.
      if (m_hs->is_entity_colliding(em, e))
      {
        pos->x = pos_snapshot.x;
      }

    }
  }

  void apply_x_movement(float norm, float x_vel, Position* pos, sf::Time dt)
  {
    pos->x += norm * x_vel * dt.asSeconds();
  }

  void apply_y_movement(float norm, float y_vel, Position* pos, sf::Time dt)
  {
    pos->y += norm * y_vel * dt.asSeconds();
  }

  void apply_friction(Velocity* vel, float friction_coef)
  {
    if (vel->x > 0.f)
  	{
  		vel->x -= friction_coef;
      vel->x = (vel->x < 0.f) ? 0.f : vel->x;
  	}
  	// Moving left
  	else
  	{
  		vel->x += friction_coef;
      vel->x = (vel->x > 0.f) ? 0.f : vel->x;
  	}
  	// Moving down
  	if (vel->y > 0.f)
  	{
  		vel->y -= friction_coef;
      vel->y = (vel->y < 0.f) ? 0.f : vel->y;
  	}
  	// Moving up
  	else
  	{
  		vel->y += friction_coef;
      vel->y = (vel->y > 0.f) ? 0.f : vel->y;
  	}
  }

  private:
  HitboxSystem* m_hs;
  const float m_DECELERATION = 1.5f;

};

#endif /* TME_PHYSICS_SYSTEM */