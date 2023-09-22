#pragma once
#ifndef TME_TILE_MAP
#define TME_TILE_MAP

#include <string>
#include <map>
#include <filesystem>

#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
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
   static constexpr Info DEFAULT_DIR_INFO = Info{Type::Directory, 16, 1, 1};

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
      construct_directory_atlas(path);
    }
  }

  void construct_directory_atlas(const std::string& path)
  {
    const std::filesystem::path tile_map_root_dir{path};

    sf::RenderTexture rt{};

    if (!rt.create({m_info.num_cols * m_info.size, m_info.num_rows * m_info.size}))
    {
        throw std::runtime_error("TileMap::construct_directory_atlas - Failed to create render texture");
    }

    std::size_t count = 0;

    for (auto const& entry : std::filesystem::directory_iterator{tile_map_root_dir})
    {
      unsigned int col = count % m_info.num_cols;
      unsigned int row = count / m_info.num_cols;

      if (entry.is_regular_file() && 
         (entry.path().extension() == ".png" || entry.path().extension() == ".jpeg" || entry.path().extension() == ".jpg"))
      {
        sf::Texture texture;

        if (!texture.loadFromFile(entry.path()))
        {
          throw std::runtime_error("TileMap::construct_directory_atlas - Failed to load " + entry.path().string());
        }

        sf::Sprite tile_sprite{texture};
        tile_sprite.setPosition({
          static_cast<float>(col * m_info.size), static_cast<float>(row * m_info.size)
        });

        rt.draw(tile_sprite);

        auto tile_name = entry.path().filename();
        m_position_mapping[tile_name.string()] = {row, col};
      }

      count++;
    }

    // Copy the texture map
    m_texture_map = rt.getTexture();
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
