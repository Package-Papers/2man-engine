#pragma once
#ifndef TME_ENGINE
#define TME_ENGINE

#include "window_context.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

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
    explicit Engine(const ushort width, const ushort height, const std::string& title,
                    const ushort target_fps = 60);
    void init_imgui();
    void init_window();

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
};
} /* namespace tme */

#endif /* TME_ENGINE */