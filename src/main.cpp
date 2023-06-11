#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

// #include <imgui-SFML.h>
// #include <imgui.h>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    // bool imgui_success = ImGui::SFML::Init(window);

    // if (!imgui_success) std::cerr << "Failed to initialize imgui.";
    sf::Clock delta_clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        // ImGui::SFML::Update(window, deltaClock.restart());

        // ImGui::ShowDemoWindow();
        window.clear();

        // ImGui::SFML::Render(window);
        window.display();
    }
    // ImGui::SFML::Shutdown();
    return 0;
}