#pragma once
#ifndef TME_MAP
#define TME_MAP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include "layer.hpp"

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

    void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default)
    {
        for (auto& layer : m_layers)
        {
            layer.draw(*m_context.window);
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
