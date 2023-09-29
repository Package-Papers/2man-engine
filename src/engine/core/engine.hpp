#pragma once
#ifndef TME_ENGINE
#define TME_ENGINE

////////////////////////////////////
// Note(Appy): This file contains the engine class. The engine class holds core components of the
// engine. This includes the main engine/game loop, the resource holder, the clock resonsible for
// computing delta time, the state stack.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include <imgui-SFML.h>
#include <imgui.h>

#include "engine/core/engine_context.hpp"
#include "engine/input/input_system.hpp"
#include "engine/resource/resources.hpp"
#include "engine/states/game_state.hpp"
#include "engine/states/menu_state.hpp"
#include "engine/states/title_state.hpp"
#include "engine/window/window_context.hpp"

#include "engine/ecs/core/entity_manager.hpp"

namespace tme
{
/**
 * @brief The core engine class.
 */
class Engine final
{
  public:
    /**
     * Default constructor is forbidden.
     * @brief Forbid default constructor.
     */
    Engine() = delete;

    /**
     * @brief Deconstructor.
     */
    ~Engine();

    /**
     * Creates a new Engine instance with the window constructed
     * according to the window context passed in.
     * @brief Constructor.
     */
    explicit Engine(const WindowContext& context);

    /**
     * @brief The main loop of the engine.
     *
     * The following core events are run: (in order)
     * - Delta time update
     * - Events handling
     * - Real time update
     * - Fixed time update
     * - Render call
     */
    void run();

  private:
    // Initializer methods.
    void register_states();
    void init_imgui();
    void init_window(const WindowContext& context);
    void init_keybinds();

    // Getter methods.
    Context get_context();

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

    // State Stack.
    StateStack m_state_stack;

    // Input components.
    std::unordered_map<std::string, sf::Keyboard::Key> m_supported_keys;
    InputSystem                                        m_input_system;
};
} /* namespace tme */

#endif /* TME_ENGINE */