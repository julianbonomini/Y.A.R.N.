#include <iostream>
#include <SFML/Graphics.hpp>

#include "apps/config/config_app.hpp"
#include "core/network/network.hpp"
#include "layout/toolbar/toolbar.hpp"
#include "layout/footer/footer.hpp"
#include "globals/ui_globals.hpp"
#include "apps/info/info_app.hpp"
#include "apps/market/market_app.hpp"
#include "apps/pomodoro/pomodoro_app.hpp"
#include "layout/main_window/main_window.hpp"
#include "core/state_machine/state_machine.hpp"
#include "ui/themes/theme_manager.hpp"


void log_separator() {
    std::cout << "***************DONE*****************" << std::endl << std::endl << std::endl;
}

void log_attention_grabber() {
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
}

int main() {
    std::cout << "Booting..." << std::endl;
    log_separator();

    // Create the StateMachine instance to manage app state.
    std::cout << "Initializing state machine from disk..." << std::endl;
    StateMachine stateMachine(0);
    OsConfigFile *os_config_file = &stateMachine.getOsConfig(); // by ref, don't copy
    OsConfigFile initial_os_config_file = stateMachine.getOsConfig(); // by ref, don't copy
    log_separator();

    auto window = sf::RenderWindow(sf::VideoMode({DisplayConfig::SCREEN_WIDTH, DisplayConfig::SCREEN_HEIGHT}), "Noop",
                                   sf::Style::Default, sf::State::Windowed);

    window.setFramerateLimit(initial_os_config_file.refreshRate);
    int loadedRefreshRate = initial_os_config_file.refreshRate; // Copies the value

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
    std::string loadaedShader = "";
    std::ostringstream shaderFile;
    std::cout << "Loading shader from disk..." << std::endl;
    shaderFile << "src/ui/shaders/" << initial_os_config_file.shader << ".frag";
    if (!crtShader.loadFromFile(shaderFile.str(), sf::Shader::Type::Fragment)) {
        log_attention_grabber();
        std::cout << "Failed to load shader. Moving without one" << std::endl;
    } else {
        loadaedShader = initial_os_config_file.shader; // Copies the value
        log_separator();
    }


    std::cout << "Initializing theme..." << std::endl;
    std::string loadedTheme = initial_os_config_file.theme; // Copies the value
    ThemeManager::instance().setTheme(loadedTheme);
    log_separator();


    std::cout << "Initializing main layout..." << std::endl;
    log_separator();
    Toolbar toolbar(renderTexture, font);
    MainWindow main_window(renderTexture, font);
    Footer footer(renderTexture, font);

    std::cout << "Initializing apps..." << std::endl;
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


        // Check if global settings have changed:
        if (os_config_file->shader != loadaedShader && loadaedShader != "") {
            std::cout << "Changing shader to " << os_config_file->shader << "..." << std::endl;
            std::ostringstream newShader;
            newShader << "src/ui/shaders/" << os_config_file->shader << ".frag";
            if (!crtShader.loadFromFile(newShader.str(), sf::Shader::Type::Fragment)) {
                log_attention_grabber();
                std::cout << "Failed to load new shader!" << std::endl;
            }
            loadaedShader = os_config_file->shader;
        }
        if (os_config_file->refreshRate != loadedRefreshRate) {
            std::cout << "Changing refresh rate to  " << os_config_file->refreshRate << "..." << std::endl;
            window.setFramerateLimit(os_config_file->refreshRate);
            loadedRefreshRate = os_config_file->refreshRate;

        }
        if (os_config_file->theme != loadedTheme) {
            std::cout << "Changing theme " << os_config_file->theme << "..." << std::endl;
            ThemeManager::instance().setTheme(os_config_file->theme);
            loadedTheme = os_config_file->theme;
        }

        // Clear screen with base color
        renderTexture.clear(ThemeManager::instance().getCurrentTheme().background());
        // Draw everything
        toolbar.draw();
        footer.draw();
        main_window.draw(apps, stateMachine.getActiveTab());
        renderTexture.display();


        // Set flicker
        float flickerFactor = 0.f;
        int flickerChance = stateMachine.getOsConfig().refreshRate * os_config_file->flickerIntensity;
        if (os_config_file->flickerEnabled && rand() % flickerChance == 0) {
            flickerFactor = 1.0f;
        }
        crtShader.setUniform("flickerFactor", flickerFactor);

        // Clear the window and draw the final image
        window.clear(ThemeManager::instance().getCurrentTheme().background());
        if (os_config_file->shaderEnabled && loadaedShader != "") {
            window.draw(shaderSprite, &crtShader); // Render the textured sprite with CRT effect
        } else {
            window.draw(shaderSprite);
        }
        window.display();
    }

    return 0;
}
