#include <iostream>
#include <SFML/Graphics.hpp>

#include "apps/config/config_app.hpp"
#include "utils/network.hpp"
#include "layout/toolbar/toolbar.hpp"
#include "layout/footer/footer.hpp"
#include "globals/theme.hpp"
#include "apps/info/info_app.hpp"
#include "apps/market/market_app.hpp"
#include "layout/main_window/main_window.hpp"
#include "core/state_machine.hpp"


int main() {
    std::cout << "Booting..." << std::endl;
    // Create the StateMachine instance to manage app state.
    std::cout << "Initializing state machine from disk..." << std::endl;
    StateMachine stateMachine(0);

    auto window = sf::RenderWindow(sf::VideoMode({Hardware::SCREEN_WIDTH, Hardware::SCREEN_HEIGHT}), "Noop", sf::Style::Close, sf::State::Windowed);
    window.setFramerateLimit(stateMachine.getOsConfig().refreshRate);
    sf::Font font;
    if (!font.openFromFile("./assets/fonts/FiraCodeNerdFont-Medium.ttf")) {
        std::cout << "Error loading default font" << std::endl;
        return 1;
    }
    sf::Text text(font);

    std::cout << "Initializing main layout..." << std::endl;
    Toolbar toolbar(window, font);
    MainWindow main_window(window, font);
    Footer footer(window, font);

    std::cout << "Initializing apps..." << std::endl;
    std::vector<std::unique_ptr<App> > apps;
    apps.push_back(std::make_unique<MarketApp>(window, font, "Stonks"));
    apps.push_back(std::make_unique<InfoApp>(window, font, "Info"));
    apps.push_back(std::make_unique<ConfigApp>(window, font, stateMachine, "Config"));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }


            // Global keys (tab switching)
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                auto activeApp = apps[stateMachine.getActiveTab()].get();
                bool activeAppOpenModal = activeApp->hasOpenModal;

                // If help modal is open, then don't take any keystrokes except Escape
                if (activeAppOpenModal && apps[stateMachine.getActiveTab()].get()->helpOpen) {
                    if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
                        apps[stateMachine.getActiveTab()].get()->settingsOpen = false;
                        apps[stateMachine.getActiveTab()].get()->helpOpen = false;
                        apps[stateMachine.getActiveTab()].get()->hasOpenModal = false;
                        // std::cout << "Help is open, closing it" << std::endl;
                    }
                    break;
                }

                // If settings modal is open, pass strokes to app expect Escape
                if (activeAppOpenModal && apps[stateMachine.getActiveTab()].get()->settingsOpen) {
                    if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
                        apps[stateMachine.getActiveTab()].get()->settingsOpen = false;
                        apps[stateMachine.getActiveTab()].get()->helpOpen = false;
                        apps[stateMachine.getActiveTab()].get()->hasOpenModal = false;
                        // std::cout << "Settings is open, closing it" << std::endl;
                        break;
                    }
                    // If not Escape, pass through to app
                    // std::cout << "Settings is open, sending strokes to app" << std::endl;
                    apps[stateMachine.getActiveTab()]->handleEvent(*keyPressed);
                    break;
                }

                // If no active modal is open, we can handle tab switching
                if (!activeAppOpenModal) {
                    // Tab left
                    if (keyPressed->scancode == sf::Keyboard::Scan::A) {
                        int new_active_tab = (stateMachine.getActiveTab() - 1 + apps.size()) % apps.size();
                        stateMachine.setActiveTab(new_active_tab);
                        // std::cout << "move left" << std::endl;
                        break;
                    }

                    // Tab right
                    if (keyPressed->scancode == sf::Keyboard::Scan::S) {
                        int new_active_tab = (stateMachine.getActiveTab() + 1) % apps.size();
                        stateMachine.setActiveTab(new_active_tab);
                        // std::cout << "move right" << std::endl;
                        break;
                    }

                    // Open help
                    if (keyPressed->scancode == sf::Keyboard::Scan::H) {
                        apps[stateMachine.getActiveTab()].get()->hasOpenModal = true;
                        apps[stateMachine.getActiveTab()].get()->helpOpen = true;
                        // std::cout << "Open Help" << std::endl;
                        break;
                    }

                    // Open settings
                    if (keyPressed->scancode == sf::Keyboard::Scan::C) {
                        apps[stateMachine.getActiveTab()].get()->hasOpenModal = true;
                        apps[stateMachine.getActiveTab()].get()->settingsOpen = true;
                        // std::cout << "Open settings" << std::endl;
                        break;
                    }

                    // std::cout << "Passthru" << std::endl;
                    apps[stateMachine.getActiveTab()]->handleEvent(*keyPressed);

                }
            }
        }
        window.clear(Colors::WHITE);

        // Draw everything
        toolbar.draw();
        footer.draw();
        main_window.draw(apps, stateMachine.getActiveTab());

        window.display();
    }

    return 0;
}
