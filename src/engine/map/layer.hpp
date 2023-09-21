#pragma once
#ifndef TME_LAYER
#define TME_LAYER

#include <chrono>
#include <iostream>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "engine/core/engine_context.hpp"
#include "engine/structures/Vec2D.hpp"

#include "engine/map/tile.hpp"

class Map;

class Layer
{
  public:
    explicit Layer(Map* map, Context ctx, std::size_t width, std::size_t height);

    // Load and cache texture
    sf::Texture* load_texture(textures::ID tid);

    void draw_tile(std::size_t x, std::size_t y);

    const Vec2D<Tile>& get() const
    {
        return m_grid;
    }

    Vec2D<Tile>& get()
    {
        return m_grid;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states);

  private:
    Map*                                           m_parent_map;
    Context                                        m_context;
    Vec2D<Tile>                                    m_grid;
    sf::RectangleShape                             m_shape;
    std::unordered_map<textures::ID, sf::Texture*> m_texture_cache;
    sf::VertexArray                                m_vertex_array;

};

#endif /* TME_LAYER */
