#pragma once
#ifndef TME_COMPONENTS
#define TME_COMPONENTS

#include "core/entity.hpp"
#include <functional>

class EntityManager;

struct Position
{
    float x;
    float y;
};

struct Controllable
{
};

struct Colour
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Interactable
{
    std::function<void(EntityManager* m_entity_manager, EntityID e1, EntityID e2)> action;
    bool  is_switch = false;
    float radius_x  = 100;
    float radius_y  = 100;
};

#endif /* TME_COMPONENTS */