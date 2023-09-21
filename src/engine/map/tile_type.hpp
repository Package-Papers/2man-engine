#pragma once
#ifndef TME_TILE_TYPE
#define TME_TILE_TYPE

#include "engine/resource/resources.hpp"

// Contains information about the type of tile.
struct TileType
{
  textures::ID m_sprite_texture_id = textures::ID::Placeholder;
};

namespace tile_types
{
constexpr TileType PLACE_HOLDER_TILE = TileType{textures::ID::Placeholder};
constexpr TileType FLOOR_TILE = TileType{textures::ID::Tile1};
}

#endif /* TME_TILE_TYPE */
