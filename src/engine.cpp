#include "engine.hpp"
#include "imgui.h"
#include "input.hpp"

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
        ImGui::SFML::ProcessEvent(m_window, event);
        m_mouse.handle_event(event);
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
    }
}

void tme::Engine::render()
{
    // Clear window.
    m_window.clear();

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

// Fixed time update. Things which does not strictly require fixed time update can be placed here.
// For example: GUI.
void tme::Engine::update_real_time()
{
    ImGui::SFML::Update(m_window, m_time_since_last_update);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::SetNextWindowSize(ImVec2(120, 0));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Stats", nullptr, window_flags);
    ImGui::Text("FPS: %d", static_cast<int>(1.f / m_time_since_last_update.asSeconds()));
    ImGui::Text("M Pressed: %d", m_mouse.is_pressed());
    ImGui::Text("M Held: %d", m_mouse.is_held());
    ImGui::End();
}

// Fixed time update. Things which should have a fixed time step update such as phyics etc. should
// be placed here.
void tme::Engine::fixed_update()
{
    // Update with the value of m_TIME_PER_FRAME.
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
}