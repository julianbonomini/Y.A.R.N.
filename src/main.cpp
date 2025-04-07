#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/network.hpp"
#include "layout/toolbar/toolbar.hpp"
#include "globals/theme.hpp"
#include "apps/info/info_app.hpp"
#include "layout/main_window/main_window.hpp"
#include "state_machine/state_machine.hpp"


int main() {
    std::cout << "Booting..." << std::endl;
    // Create the StateMachine instance to manage app state.
    StateMachine stateMachine(0);
    std::cout << "Initial active tab: " << stateMachine.getActiveTab() << std::endl;

    auto window = sf::RenderWindow(sf::VideoMode({800, 480}), "TickPi", sf::Style::Default, sf::State::Windowed);
    // sf::State::Fullscreen for full screen
    window.setFramerateLimit(120);
    sf::Font font;
    if (!font.openFromFile("./assets/fonts/FiraCodeNerdFont-Medium.ttf")) {
        std::cout << "Error loading default font" << std::endl;
        return 1;
    }
    sf::Text text(font);

    Toolbar toolbar(window, font);
    MainWindow main_window(window, font);

    std::vector<std::unique_ptr<App>> apps;
    apps.push_back(std::make_unique<InfoApp>(window, font, "Info"));
    apps.push_back(std::make_unique<InfoApp>(window, font, "Weather"));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                int new_active_tab = (stateMachine.getActiveTab() - 1 + apps.size()) % apps.size();
                stateMachine.setActiveTab(new_active_tab);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                int new_active_tab = (stateMachine.getActiveTab() + 1) % apps.size();
                stateMachine.setActiveTab(new_active_tab);
            }
        }
        window.clear(Theme::Background);

 		std::string ip = Network::getIp();

        toolbar.draw();
        main_window.draw(apps, stateMachine.getActiveTab());

        window.display();
    }

    return 0;
}