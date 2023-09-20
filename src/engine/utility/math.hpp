#pragma once
#ifndef TME_MATH
#define TME_MATH

#include "SFML/System/Vector2.hpp"
#include <cmath>

namespace tme
{
inline sf::Vector2f normalize(const sf::Vector2f&& vec)
{
  if (vec.x == 0.f && vec.y == 0.f) return {};
  auto n = static_cast<float>(sqrt(pow(vec.x, 2.0) + pow(vec.y, 2.0)));
  return vec/n;
}

inline sf::Vector2f normalize(float x, float y)
{
  return normalize({x, y});
}
}

#endif /* TME_MATH */
