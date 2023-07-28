#pragma once
#ifndef TME_ENGINE
#define TME_ENGINE

////////////////////////////////////
// Note(Appy): This file contains the engine class. The engine class holds core components of the
// engine. This includes the main engine/game loop, the resource holder, the clock resonsible for
// computing delta time, the state stack.

#include <iostream>

#include "resources.hpp"
#include "window_context.hpp"
#include <imgui-SFML.h>
#include <imgui.h>

#include "mouse.hpp"
#include "window_context.hpp"

namespace tme
{
class Engine final
{
  public:
    // No default construction.
    Engine() = delete;
    ~Engine();
    explicit Engine(const WindowContext& context);
    void run();

  private:
    // Initializer methods.
    void init_imgui();
    void init_window(const WindowContext& context);

    // Update methods.
    void handle_events();
    void update_dt();
    void update_real_time();
    void update_fixed_time();
    void fixed_update();

    void render();

  private:
    // Window and context.
    sf::VideoMode    m_video_mode;
    sf::RenderWindow m_window;
    ushort           m_target_fps;

    // Time and clock.
    sf::Clock      m_delta_clock;
    sf::Time       m_time_since_last_update = sf::Time::Zero;
    const sf::Time m_TIME_PER_FRAME;

    // Resource holders.
    resource_holder::TextureHolder m_texture_holder;
    resource_holder::FontHolder    m_font_holder;

    // Input components.
    tme::Mouse m_mouse;
};
} /* namespace tme */

#endif /* TME_ENGINE */