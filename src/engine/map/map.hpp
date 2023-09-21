#pragma once
#ifndef TME_MAP
#define TME_MAP


#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/map/layer.hpp"

class Map
{
  private:
    friend Layer;

  public:
    explicit Map(Context ctx, std::size_t width, std::size_t height, std::size_t layers = 1)
        : m_layers(layers, Layer(this, ctx, width, height))
        , m_size{static_cast<unsigned int>(width), static_cast<unsigned int>(height)}
        , m_context{ctx}
    {
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default)
    {
      for (auto& layer : m_layers)
      {
          layer.draw(target, states);
      }
    }

    template <std::size_t layer>
    [[nodiscard]] const Tile at(std::size_t row, std::size_t col) const
    {
      return m_layers[layer].get().at(row, col);
    }

    template <std::size_t layer>
    [[nodiscard]] Tile& at(std::size_t row, std::size_t col)
    {
      return m_layers[layer].get().at(row, col);
    }

    template <std::size_t layer>
    void set(std::size_t row, std::size_t col,
                           const TileType type = tile_types::PLACE_HOLDER_TILE)
    {
      auto& tile  = m_layers[layer].get().at(row, col);
      tile.m_type = type;

      // Note: remove this later
      if (type.m_sprite_texture_id == tile_types::PLACE_HOLDER_TILE.m_sprite_texture_id)
      {
          tile.m_texture = nullptr;
      }
      else
      {
          tile.m_texture = &m_context.textures->get(type.m_sprite_texture_id);
      }
    }

    [[nodiscard]] const sf::Vector2u dims()
    {
      return m_size;
    }

  private:
    std::vector<Layer> m_layers;
    Context            m_context;
    sf::Vector2u       m_size;
};

#endif /* TME_MAP */
