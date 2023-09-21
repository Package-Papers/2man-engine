#pragma once
#ifndef TME_TILE
#define TME_TILE

#include <SFML/System/Vector2.hpp>

#include "engine/map/tile_type.hpp"
#include "engine/resource/resources.hpp"

constexpr float TILE_SIZE = 16.f;

struct Tile
{
    TileType     m_type;
    sf::Vector2u m_pos;
    sf::Texture* m_texture;
};

#endif /* TME_TILE */
