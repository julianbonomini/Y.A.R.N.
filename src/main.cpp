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
#include "apps/weather/weather_app.hpp"
#include "layout/main_window/main_window.hpp"
#include "core/state_machine/state_machine.hpp"
#include "ui/themes/theme_manager.hpp"
#include "common/logger.hpp"


int main() {
    Logger::info("Booting...");
    Logger::done_separator();

    // Create the StateMachine instance to manage app state.
    Logger::info("Initializing state machine from disk...");
    StateMachine stateMachine(0);
    OsConfigFile *os_config_file = &stateMachine.getOsConfig(); // by ref, don't copy
    OsConfigFile initial_os_config_file = stateMachine.getOsConfig(); // by ref, don't copy
    Logger::done_separator();

    auto window = sf::RenderWindow(sf::VideoMode({DisplayConfig::SCREEN_WIDTH, DisplayConfig::SCREEN_HEIGHT}), "Noop",
                                   sf::Style::Default, sf::State::Windowed);

    window.setFramerateLimit(initial_os_config_file.refreshRate);
    int loadedRefreshRate = initial_os_config_file.refreshRate; // Copies the value

    sf::Font font;
    if (!font.openFromFile("./assets/fonts/PxPlus_IBM_VGA8.ttf")) {
        Logger::error("Error loading default font");
        return 1;
    }
    sf::Text text(font);

    // Create a render texture with the same size as the window
    sf::RenderTexture renderTexture({DisplayConfig::SCREEN_WIDTH, DisplayConfig::SCREEN_HEIGHT});

    // Set up the sprite for rendering the texture
    sf::Sprite shaderSprite(renderTexture.getTexture());

    if (!sf::Shader::isAvailable()) {
        Logger::error("Shader is not available.");
        return 1;
    }

    Logger::info("Loading shader from disk...");
    sf::Shader crtShader;
    std::string loadaedShader = "";
    std::ostringstream shaderFile;
    shaderFile << "src/ui/shaders/" << initial_os_config_file.shader << ".frag";
    if (!crtShader.loadFromFile(shaderFile.str(), sf::Shader::Type::Fragment)) {
        Logger::warning("Failed to load shader. Moving without one");
    } else {
        loadaedShader = initial_os_config_file.shader; // Copies the value
        Logger::done_separator();
    }


    Logger::info("Initializing theme...");
    std::string loadedTheme = initial_os_config_file.theme; // Copies the value
    ThemeManager::instance().setTheme(loadedTheme);
    Logger::done_separator();


    Logger::info("Initializing main layout...");
    Toolbar toolbar(renderTexture, font);
    MainWindow main_window(renderTexture, font);
    Footer footer(renderTexture, font);
    Logger::done_separator();

    Logger::info("Initializing apps...");
    std::vector<std::unique_ptr<App> > apps;
    // These are ordered.
    apps.push_back(std::make_unique<MarketApp>("MKT", renderTexture, font, stateMachine));
    apps.push_back(std::make_unique<PomodoroApp>("PMD", renderTexture, font, stateMachine));
    apps.push_back(std::make_unique<WeatherApp>("WTH", renderTexture, font, stateMachine));
    apps.push_back(std::make_unique<InfoApp>("INF", renderTexture, font));
    apps.push_back(std::make_unique<ConfigApp>("CNF", renderTexture, font, stateMachine, apps.size() + 1));
    Logger::info("Apps booted successfully...");
    Logger::done_separator();

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
                        break;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scan::H) {
                        activeApp->setSettingsOpen(false);
                        activeApp->setHelpOpen(false);
                        activeApp->setHasOpenModal(false);
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
                        break;
                    }

                    // Tab right
                    if (keyPressed->scancode == sf::Keyboard::Scan::S) {
                        int new_active_tab = (stateMachine.getActiveTab() + 1) % apps.size();
                        stateMachine.setActiveTab(new_active_tab);
                        break;
                    }

                    // Open help
                    if (keyPressed->scancode == sf::Keyboard::Scan::H) {
                        activeApp->setHasOpenModal(true);
                        activeApp->setHelpOpen(true);
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

                    apps[stateMachine.getActiveTab()]->handleEvent(*keyPressed);
                }
            }
        }


        // Check if global settings have changed:
        if (os_config_file->shader != loadaedShader && loadaedShader != "") {
            Logger::info("Changing shader to", os_config_file->shader, "...");
            std::ostringstream newShader;
            newShader << "src/ui/shaders/" << os_config_file->shader << ".frag";
            if (!crtShader.loadFromFile(newShader.str(), sf::Shader::Type::Fragment)) {
                Logger::error("Failed to load new shader!");
            } else {
                loadaedShader = os_config_file->shader;
            }
        }
        if (os_config_file->refreshRate != loadedRefreshRate) {
            Logger::info("Changing refresh rate to", os_config_file->refreshRate, "...");
            window.setFramerateLimit(os_config_file->refreshRate);
            loadedRefreshRate = os_config_file->refreshRate;
        }
        if (os_config_file->theme != loadedTheme) {
            Logger::info("Changing theme to", os_config_file->theme, "...");
            ThemeManager::instance().setTheme(os_config_file->theme);
            loadedTheme = os_config_file->theme;
        }

        // Clear screen with base color
        renderTexture.clear(ThemeManager::instance().getCurrentTheme().background());
        // Draw everything
        toolbar.draw();
        footer.draw();
        main_window.draw(apps, stateMachine);
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
