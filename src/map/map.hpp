#pragma once
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#ifndef TME_MAP
#define TME_MAP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "layer.hpp"

class Map : public sf::Drawable
{
  public:
    explicit Map(std::size_t width, std::size_t height, std::size_t layers = 1)
        : m_layers(layers, Layer(width, height))
    {
    }

    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        for (const auto& layer : m_layers)
        {
            target.draw(layer, states);
        }
    }

    template <std::size_t layer>
    [[nodiscard]] const textures::ID at(std::size_t row, std::size_t col) const
    {
        return m_layers[layer].get().at(row, col);
    }

    template <std::size_t layer>
    [[nodiscard]] textures::ID& at(std::size_t row, std::size_t col)
    {
        return m_layers[layer].get().at(row, col);
    }

  private:
    std::vector<Layer> m_layers;
};

#endif /* TME_MAP */
