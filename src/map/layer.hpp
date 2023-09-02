#pragma once
#ifndef TME_LAYER
#define TME_LAYER

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "../structures/Vec2D.hpp"

class Layer : public sf::Drawable
{
public:
  explicit Layer(std::size_t width, std::size_t height)
    : m_grid(width, height)
  {}

  void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override
  {
    sf::RectangleShape box;
    auto size = sf::Vector2f{20.f, 20.f};

    box.setSize(size);
    box.setOutlineThickness(0.5f);

    const auto [width, height] = m_grid.dim();

    for (auto y=0; y<height; y++)
    {
      for (auto x=0; x<width; x++)
      {
        if (m_grid.at(y, x) == 0) continue;

        switch (m_grid.at(y, x))
        {
          break; case 1: box.setFillColor(sf::Color::Red);
          break; case 2: box.setFillColor(sf::Color::Green);
          break; default: continue;
        }

        auto pos = sf::Vector2f{static_cast<float>(x), static_cast<float>(y)};
        box.setPosition(pos * size.x);
        target.draw(box, states);
      }
    }
  }

  const Vec2D<int>& get() const
  {
    return m_grid;
  }

  Vec2D<int>& get()
  {
    return m_grid;
  }

private:
  Vec2D<int> m_grid;
};

#endif /* TME_LAYER */
