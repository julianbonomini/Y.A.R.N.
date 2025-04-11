#include <iostream>
#include <SFML/Graphics.hpp>

#include "apps/config/config_app.hpp"
#include "core/network/network.hpp"
#include "layout/toolbar/toolbar.hpp"
#include "layout/footer/footer.hpp"
#include "globals/theme.hpp"
#include "apps/info/info_app.hpp"
#include "apps/market/market_app.hpp"
#include "apps/pomodoro/pomodoro_app.hpp"
#include "layout/main_window/main_window.hpp"
#include "core/state_machine/state_machine.hpp"


void log_separator() {
    std::cout << "************************************" << std::endl << std::endl << std::endl;
}

void log_attention_grabber() {
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
}

int main() {
    std::cout << "Booting..." << std::endl;
    log_separator();

    // Create the StateMachine instance to manage app state.
    std::cout << "Initializing state machine from disk..." << std::endl;
    log_separator();
    StateMachine stateMachine(0);
    OsConfigFile os_config_file  = stateMachine.getOsConfig();

    auto window = sf::RenderWindow(sf::VideoMode({DisplayConfig::SCREEN_WIDTH, DisplayConfig::SCREEN_HEIGHT}), "Noop",
                                   sf::Style::Default, sf::State::Windowed);
    window.setFramerateLimit(os_config_file.refreshRate);
    sf::Font font;
    if (!font.openFromFile("./assets/fonts/PxPlus_IBM_VGA8.ttf")) {
        log_attention_grabber();
        std::cout << "Error loading default font" << std::endl;
        log_attention_grabber();
        return 1;
    }
    sf::Text text(font);

    // Create a render texture with the same size as the window
    sf::RenderTexture renderTexture({DisplayConfig::SCREEN_WIDTH, DisplayConfig::SCREEN_HEIGHT});

    // Set up the sprite for rendering the texture
    sf::Sprite shaderSprite(renderTexture.getTexture());

    if (!sf::Shader::isAvailable()) {
        log_attention_grabber();
        std::cout << "No shaders available" << std::endl;
        return 1;
    }

    sf::Shader crtShader;
    std::ostringstream shaderFile;
    shaderFile << "src/ui/shaders/" << os_config_file.shader << ".frag";
    if (!crtShader.loadFromFile(shaderFile.str(), sf::Shader::Type::Fragment)) {
        std::cout << "Failed to load shader!" << std::endl;
        return 1;
    }

    std::cout << "Initializing main layout..." << std::endl;
    log_separator();
    Toolbar toolbar(renderTexture, font);
    MainWindow main_window(renderTexture, font);
    Footer footer(renderTexture, font);

    std::cout << "Initializing apps..." << std::endl;
    log_separator();
    std::vector<std::unique_ptr<App> > apps;
    // These are ordered.
    apps.push_back(std::make_unique<MarketApp>("MKT", renderTexture, font, stateMachine));
    apps.push_back(std::make_unique<PomodoroApp>("PMD", renderTexture, font, stateMachine));
    apps.push_back(std::make_unique<ConfigApp>("CNF", renderTexture, font, stateMachine));
    apps.push_back(std::make_unique<InfoApp>("INF", renderTexture, font));

    std::cout << "Apps booted successfully..." << std::endl;
    log_separator();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }


            // Global keys (tab switching)
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                auto activeApp = apps[stateMachine.getActiveTab()].get();
                bool activeAppOpenModal = activeApp->getHasOpenModal();

                // If help modal is open, then don't take any keystrokes except Escape
                if (activeAppOpenModal && activeApp->isHelpOpen()) {
                    if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
                        activeApp->setSettingsOpen(false);
                        activeApp->setHelpOpen(false);
                        activeApp->setHasOpenModal(false);
                        // std::cout << "Help is open, closing it" << std::endl;
                        break;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scan::H) {
                        activeApp->setSettingsOpen(false);
                        activeApp->setHelpOpen(false);
                        activeApp->setHasOpenModal(false);
                        // std::cout << "Help is open, closing it" << std::endl;
                        break;
                    }
                    break;
                }

                // If settings modal is open, pass strokes to app expect Escape
                if (activeAppOpenModal && activeApp->isSettingsOpen()) {
                    activeApp->handleEvent(*keyPressed);
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
                        activeApp->setHasOpenModal(true);
                        activeApp->setHelpOpen(true);
                        // std::cout << "Open Help" << std::endl;
                        break;
                    }

                    // Open settings
                    if (keyPressed->scancode == sf::Keyboard::Scan::C) {
                        // Check if app accepts config
                        if (dynamic_cast<AppWithConfig *>(activeApp)) {
                            activeApp->setHasOpenModal(true);
                            activeApp->setSettingsOpen(true);
                            break;
                        }
                        break;
                    }

                    // std::cout << "Passthru" << std::endl;
                    apps[stateMachine.getActiveTab()]->handleEvent(*keyPressed);
                }
            }
        }


        // Clear screen with base color
        renderTexture.clear(Colors::WHITE);
        // Draw everything
        toolbar.draw();
        footer.draw();
        main_window.draw(apps, stateMachine.getActiveTab());
        renderTexture.display();


        // Set flicker
        float flickerFactor = 0.f;
        int flickerChance = stateMachine.getOsConfig().refreshRate * os_config_file.flickerIntensity;
        if (os_config_file.flickerToggle && rand() % flickerChance == 0) {
            flickerFactor = 1.0f;
        }
        crtShader.setUniform("flickerFactor", flickerFactor);

        // Clear the window and draw the final image
        window.clear(Colors::WHITE);
        if (os_config_file.shaderToggle) {
            window.draw(shaderSprite, &crtShader); // Render the textured sprite with CRT effect
        } else {
            window.draw(shaderSprite);
        }
        window.display();
    }

    return 0;
}
