#include "engine.hpp"

tme::Engine::Engine(const WindowContext& context)
    : m_video_mode({context.width, context.height})
    , m_window(m_video_mode, context.title,
               context.fullscreen ? sf::Style::Fullscreen : sf::Style::Default)
    , m_target_fps(context.target_fps)
    , m_TIME_PER_FRAME(sf::seconds(1.f / static_cast<float>(m_target_fps)))
{
    // Apply window settings
    this->init_window(context);

    // Initialization goes here...
    this->init_imgui();

    this->init_keybinds();

    this->m_state_stack.set_context(get_context());

    this->m_font_holder.load(fonts::ID::Aerial, "font.ttf");
    this->m_font_holder.get(fonts::ID::Aerial);

    this->register_states();
}

// Read in the context and applies it to the window created for the engine.
void tme::Engine::init_window(const WindowContext& context)
{
    // Set FPS limit.
    bool has_fps_limit = (context.target_fps != 0);

    if (has_fps_limit)
    {
        m_window.setFramerateLimit(context.target_fps);
    }
    else if (context.vsync)
    {
        // Does not play well when there is a fps limit.
        m_window.setVerticalSyncEnabled(true);
    }
}

// This reads a file with the following format: `KEYWORD KEY_ID`
// KEYWORD is a string representation of what the key is (fullcaps), for example,
// the key 'p' would be 'P' and the spacebar key would be 'SPACEBAR'.
// This function creates a mapping of key names to their corresponding sf::Keyboard::Key value.
void tme::Engine::init_keybinds()
{
    std::string   path = "meta/keybinds/supported_keybinds.ini";
    std::ifstream ifs(path);

    if (ifs.is_open())
    {
        std::string key{};
        int         keycode;

        while (ifs >> key >> keycode)
        {
            m_supported_keys.try_emplace(key, static_cast<sf::Keyboard::Key>(keycode));

#ifndef NDEBUG
            std::cout << "Binded: [" << key << "]-[" << keycode << "]\n";
#endif
        }
    }
    else
    {
        throw std::runtime_error("Supported keys file path invalid: " + path);
    }

    ifs.close();

    m_keyboard.set_keybinds(m_supported_keys);
}

void tme::Engine::register_states()
{
    m_state_stack.register_state<TitleState>(states::TitleScreen);
    m_state_stack.register_state<MenuState>(states::MainMenu);
    m_state_stack.register_state<GameState>(states::Game);

    auto title_state = m_state_stack.create_state(states::TitleScreen);
    m_state_stack.push_state(std::move(title_state));
}

void tme::Engine::init_imgui()
{
    bool imgui_success = ImGui::SFML::Init(this->m_window);
    if (!imgui_success)
        std::cerr << "Failed to initialize imgui.";
}

tme::Engine::~Engine()
{
    // Stop imgui
    ImGui::SFML::Shutdown();
}

// Handle SFML events.
void tme::Engine::handle_events()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_mouse.handle_event(event);
        ImGui::SFML::ProcessEvent(m_window, event);
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
        m_state_stack.handle_event(event);
    }
}

void tme::Engine::render()
{
    // Clear window.
    m_window.clear();

    m_state_stack.draw();

    // Render stuff here.
    ImGui::SFML::Render(m_window);

    // Display window.
    m_window.display();
}

// Update delta time.
void tme::Engine::update_dt()
{
    m_time_since_last_update = m_delta_clock.restart();
}

// Apply the fixed update N times, where N is (m_TIME_PER_FRAME)//(m_time_since_last_update). This
// ensures that every thing stays consistent.
void tme::Engine::update_fixed_time()
{
    while (m_time_since_last_update >= m_TIME_PER_FRAME)
    {
        m_time_since_last_update -= m_TIME_PER_FRAME;
        this->fixed_update();
    }
}

// Real time update. Things which does not strictly require fixed time update can be placed here.
// For example: GUI.
void tme::Engine::update_real_time()
{
    m_keyboard.update();
    m_mouse.update_position(m_window);
    ImGui::SFML::Update(m_window, m_time_since_last_update);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::SetNextWindowSize(ImVec2(140, 0));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Stats", nullptr, window_flags);
    ImGui::Text("FPS: %d", static_cast<int>(1.f / m_time_since_last_update.asSeconds()));
    // ImGui::Text("M Pressed: %d", m_mouse.is_pressed());
    // ImGui::Text("M Held: %d", m_mouse.is_held());
    // auto pos = m_mouse.get_position();
    // ImGui::Text("M Pos: <%d, %d>", pos.x, pos.y);

    ImGui::End();

    m_state_stack.update(m_time_since_last_update);
}

// Fixed time update. Things which should have a fixed time step update such as phyics etc. should
// be placed here.
void tme::Engine::fixed_update()
{
    // Update with the value of m_TIME_PER_FRAME.
}

Context tme::Engine::get_context()
{
    return {&m_window, &m_texture_holder, &m_font_holder, &m_keyboard, &m_mouse};
}

// The main engine loop.
void tme::Engine::run()
{
    while (m_window.isOpen())
    {
        // Update clock and record the delta time.
        this->update_dt();

        // Update events.
        this->handle_events();

        // Real time updates
        this->update_real_time();

        // Fixed time updates
        this->update_fixed_time();

        // Render
        this->render();
    }

    ImGui::SFML::Shutdown();
}