#pragma once
#ifndef TME_COMPONENTS
#define TME_COMPONENTS

struct Position
{
    float x;
    float y;
};

struct Controllable
{
};

struct Interactable
{
    bool  is_switch = false;
    float radius_x  = 100;
    float radius_y  = 100;
};

#endif /* TME_COMPONENTS */