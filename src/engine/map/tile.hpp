#pragma once
#ifndef TME_TILE
#define TME_TILE

#include <SFML/System/Vector2.hpp>

#include "engine/map/tile_type.hpp"
#include "engine/resource/resources.hpp"

struct Tile
{
    TileType     m_type;
    sf::Vector2i m_pos;
    textures::ID m_sprite_texture_id;
};

#endif /* TME_TILE */
