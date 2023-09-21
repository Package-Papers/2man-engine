#include "layer.hpp"
#include "SFML/Graphics/Color.hpp"
#include "map.hpp"

Layer::Layer(Map* map, Context ctx, std::size_t width, std::size_t height)
    : m_parent_map(map)
    , m_context{ctx}
    , m_grid(width, height)
{
    auto size = sf::Vector2f{20.f, 20.f};
    m_shape.setSize(size);
    m_shape.setOutlineThickness(0.5f);
}

void Layer::draw_tile(std::size_t x, std::size_t y)
{
    // Set tile position
    auto pos = sf::Vector2f{static_cast<float>(x), static_cast<float>(y)};
    m_shape.setPosition(pos * m_shape.getSize().x);

    // Set tile texture
    auto tid = m_grid.at(y, x).m_type.m_sprite_texture_id;

    auto texture = load_texture(textures::ID::Tile1);
    m_shape.setTexture(texture);

    // Draw the tile
    m_context.window->draw(m_shape);
}

sf::Texture* Layer::load_texture(textures::ID tid)
{
    if (m_texture_cache.contains(tid))
    {
        return m_texture_cache[tid];
    }
    else
    {
        auto& texture        = m_context.textures->get(tid);
        m_texture_cache[tid] = &texture;
        return &texture;
    }
}
