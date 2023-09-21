#pragma once
#ifndef TME_TILE_MAP
#define TME_TILE_MAP

#include <string>
#include <map>

#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Texture.hpp"
/**
 * @brief A wrapper class for a tilemap texture pack.
 */

class TileMap
{
public:

  enum class Type
  {
    Atlas,
    Directory
  };

  struct Info
  {
    Type type;

    // The dimension of each tile.
    unsigned int size;

    // The didmensions of the tilemap
    unsigned int num_rows;
    unsigned int num_cols;

  };

   static constexpr Info DEFAULT_INFO = Info{Type::Atlas, 16, 1, 1};

public:

  TileMap(const std::string& path, Info info = DEFAULT_INFO)
  : m_info(info)
  {
    if (info.type == Type::Atlas)
    {
      construct_atlas(path);
    }
    else
    {
      // Directory mode
    }
  }


  void construct_atlas(const std::string& path)
  {
    if (!m_texture_map.loadFromFile(path))
        throw std::runtime_error("TileMap::construct_atlas - Failed to load " + path);

    for (unsigned int row = 0; row < m_info.num_rows; row++)
    {
      for (unsigned int col = 0; col < m_info.num_cols; col++)
      {
        auto tile_name = "tile_"+std::to_string(row * m_info.num_rows + col);
        m_position_mapping[tile_name] = {row, col};
      }
    }
  }

  void set_vertex(const std::string& tile_name, sf::Vertex* vertices)
  {
    if (!m_position_mapping.contains(tile_name))
    {
      throw std::runtime_error("TileMap::set_vertex - Tile with given name not found: " + tile_name);
    }

    auto [x, y] = m_position_mapping.at(tile_name);

    vertices[0].texCoords = sf::Vector2f(x * m_info.size, y * m_info.size);
    vertices[1].texCoords = sf::Vector2f((x + 1) * m_info.size, y * m_info.size);
    vertices[2].texCoords = sf::Vector2f(x * m_info.size, (y + 1) * m_info.size);
    vertices[3].texCoords = sf::Vector2f(x * m_info.size, (y + 1) * m_info.size);
    vertices[4].texCoords = sf::Vector2f((x + 1) * m_info.size, y * m_info.size);
    vertices[5].texCoords = sf::Vector2f((x + 1) * m_info.size, (y + 1) * m_info.size);
  }

private:
  sf::Texture m_texture_map;
  Info m_info;
  std::map<std::string, sf::Vector2u> m_position_mapping;
};

#endif /* TME_TILE_MAP */
