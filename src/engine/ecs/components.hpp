#pragma once
#ifndef TME_COMPONENTS
#define TME_COMPONENTS

#include <functional>
#include <string>

#include "core/entity.hpp"

namespace textures
{
enum class ID;
}

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

struct Button
{
    std::string text;

    enum State
    {
        active = 0,
        hover  = 1,
        idle   = 2
    };

    State                 state;
    std::function<void()> action;

    float        size_x;
    float        size_y;
    textures::ID texture_ID;
};

struct Interactable
{
    std::function<void(EntityManager* m_entity_manager, EntityID e1, EntityID e2)> action;
    bool  is_switch = false;
    float radius_x  = 100;
    float radius_y  = 100;
};

struct Hitbox
{
    bool  active;
    float width;
    float height;
    float x_offset;
    float y_offset;
};

struct Velocity
{
    float x;
    float y;
    float max;


    void apply(float vx, float vy)
    {
        x += vx;
        y += vy;

        if (x < -max) x = -max;
        if (x > max) x = max;
        if (y < -max) y = -max;
        if (y > max) y = max;
    }
};

#endif /* TME_COMPONENTS */