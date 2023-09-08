#pragma once
#include "SFML/System/Vector3.hpp"
#ifndef TME_LAYER
#define TME_LAYER

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../structures/Vec2D.hpp"
#include "tile.hpp"
#include "../state.hpp"

class Map;

class Layer
{
  public:
    explicit Layer(Map* map, State::Context ctx, std::size_t width, std::size_t height);

    // Load and cache texture
    sf::Texture* load_texture(textures::ID tid);

    void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default)
    {
        const auto [width, height] = m_grid.dim();

        for (auto y = 0; y < height; y++)
        {
            for (auto x = 0; x < width; x++)
            {
                if (m_grid.at(y, x).m_sprite_texture_id == textures::ID::Placeholder)
                    continue;

                draw_tile(x, y);
            }
        }
    }

    void draw_tile(std::size_t x, std::size_t y);

    const Vec2D<Tile>& get() const
    {
        return m_grid;
    }

    Vec2D<Tile>& get()
    {
        return m_grid;
    }

  private:
    Map*                                           m_parent_map;
    State::Context                                 m_context;
    Vec2D<Tile>                                    m_grid;
    sf::RectangleShape                             m_shape;
    std::unordered_map<textures::ID, sf::Texture*> m_texture_cache;
};

#endif /* TME_LAYER */
