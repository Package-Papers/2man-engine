#include "layer.hpp"
#include "SFML/Graphics/Color.hpp"
#include "map.hpp"

Layer::Layer(Map* map, Context ctx, std::size_t width, std::size_t height)
    : m_parent_map(map)
    , m_context{ctx}
    , m_grid(width, height)
{
    auto size = sf::Vector2f{TILE_SIZE, TILE_SIZE};
    m_shape.setSize(size);
    m_shape.setOutlineThickness(0.5f);

    m_vertex_array.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertex_array.resize(width * height * 6);


    for (std::size_t r = 0; r < height; r++)
    {
        for (std::size_t c = 0; c < width; c++)
        {
            // Set tile position
            m_grid.at(r, c).m_pos = {static_cast<unsigned int>(c), static_cast<unsigned int>(r)};

            sf::Vertex* triangles = &m_vertex_array[(c + r * width) * 6];

            triangles[0].position = sf::Vector2f(c * TILE_SIZE, r * TILE_SIZE);
            triangles[1].position = sf::Vector2f((c + 1) * TILE_SIZE, r * TILE_SIZE);
            triangles[2].position = sf::Vector2f(c * TILE_SIZE, (r + 1) * TILE_SIZE);
            triangles[3].position = sf::Vector2f(c * TILE_SIZE, (r + 1) * TILE_SIZE);
            triangles[4].position = sf::Vector2f((c + 1) * TILE_SIZE, r * TILE_SIZE);
            triangles[5].position = sf::Vector2f((c + 1) * TILE_SIZE, (r + 1) * TILE_SIZE);

            triangles[0].texCoords = sf::Vector2f(0 * TILE_SIZE, 0 * TILE_SIZE);
            triangles[1].texCoords = sf::Vector2f((0 + 1) * TILE_SIZE, 0 * TILE_SIZE);
            triangles[2].texCoords = sf::Vector2f(0 * TILE_SIZE, (0 + 1) * TILE_SIZE);
            triangles[3].texCoords = sf::Vector2f(0 * TILE_SIZE, (0 + 1) * TILE_SIZE);
            triangles[4].texCoords = sf::Vector2f((0 + 1) * TILE_SIZE, 0 * TILE_SIZE);
            triangles[5].texCoords = sf::Vector2f((0 + 1) * TILE_SIZE, (0 + 1) * TILE_SIZE);
        }
    }
}

//void Layer::draw_tile(std::size_t x, std::size_t y)
//{
//    const auto& tile = m_grid.at(y, x);
//
//    // Set tile position
//    m_shape.setPosition(
//        sf::Vector2f{static_cast<float>(tile.m_pos.x), static_cast<float>(tile.m_pos.y)} * TILE_SIZE
//    );
//
//    // Set tile texture
//    auto texture = tile.m_texture;
//
//    if (texture != nullptr)
//    {
//        m_shape.setTexture(texture);
//
//        // Draw the tile
//        m_context.window->draw(m_shape);
//    }
//}

void Layer::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.texture = load_texture(textures::ID::Tile1);
    target.draw(m_vertex_array, states);
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
