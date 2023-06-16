#pragma once
#ifndef TME_ENGINE
#define TME_ENGINE

#include "window_context.hpp"
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
                    const ushort target_fps);
    void init_imgui();
    void init_window();

    void handle_events();
    void tick();

    void render();

  private:
    sf::VideoMode    m_video_mode;
    sf::RenderWindow m_window;
    ushort           m_target_fps;
    sf::Clock        m_delta_clock;
};
} // namespace tme

#endif /* TME_ENGINE */
