#pragma once
#ifndef TME_ENGINE_CONTEXT
#define TME_ENGINE_CONTEXT

namespace sf
{
  class RenderWindow;
}

namespace tme
{
  class Keyboard;
  class Mouse;
}

namespace sf
{
  class Texture;
}

namespace textures
{
  enum class ID;
}

#include "resources.hpp"

struct Context
{
    sf::RenderWindow*               window;
    resource_holder::TextureHolder* textures;
    resource_holder::FontHolder*    fonts;
    tme::Keyboard*                  keyboard;
    tme::Mouse*                     mouse;
};

#endif /* TME_ENGINE_CONTEXT */
