#include "engine.hpp"

tme::Engine::Engine(const ushort width, const ushort height, const std::string& title,
                    const ushort target_fps)
    : m_video_mode({width, height})
    , m_window(m_video_mode, title)
    , m_target_fps(target_fps)
{
}

tme::Engine::Engine(const WindowContext& context)
    : Engine(context.width, context.height, context.title, context.m_target_fps)
{
    // Initialization goes here...
    init_imgui();
}

void tme::Engine::init_window()
{
    m_window.setFramerateLimit(m_target_fps);
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
void tme::Engine::handle_events()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
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

void tme::Engine::tick()
{
    // Update events.
    this->handle_events();

    // Things to update goes here.
    ImGui::SFML::Update(m_window, m_delta_clock.restart());
}
void tme::Engine::run()
{
    while (m_window.isOpen())
    {
        this->tick();
        this->render();
    }
}