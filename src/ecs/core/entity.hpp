#pragma once
#ifndef TME_ECS_ENTITY
#define TME_ECS_ENTITY

#include <stdint.h>
#include <vector>

#include "../../bitmask.hpp"
#include "component.hpp"

using EntityID                   = uint64_t;
constexpr uint8_t MAX_COMPONENTS = 64;
using ComponentMask              = tme::Bitmask<MAX_COMPONENTS>;

struct EntityInfo
{
    EntityID      m_id;
    ComponentMask m_mask;
};

#endif /* TME_ECS_ENTITY */
