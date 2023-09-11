// ecs.hpp
#pragma once
#ifndef TME_ECS
#define TME_ECS

/**
 * @brief This serves as an all in one header file for using ECS.
 * 
 * ECS stands for the Entity Component System. In an ECS, an
 * entity is just an ID and components are PODs (plain old data).
 * Components can be attached and detached from entities dynamically.
 * Entities will automatically registered into systems if they satisfy
 * the component requirements. You do not need to worry about wiring it up.
 * 
 * The following example shows how to use the ECS. 
 *
 * -------------------
 * Creating an entity:
 * -------------------
 *
 * EntityManager em;
 * auto entity = em.create_entity();
 *
 * -----------------------------------
 * Attaching a component to an entity:
 * -----------------------------------
 * 
 * struct Health { int hp; };
 * em.attach<Health>(entity);
 *
 * ----------------
 * Making a system:
 * ----------------
 * 
 * // Perhaps an 'on fire' system:
 * for (auto entity : EntityCapture<Health>(entity_manager))
 * {
 *   auto health = entity_manager.get<Health>(entity);
 *   health->hp -= 1;
 * }
 *
 */

#include "systems/systems.hpp"

#endif /* TME_ECS */
