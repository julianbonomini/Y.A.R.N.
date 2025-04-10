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

    auto window = sf::RenderWindow(sf::VideoMode({DisplayConfig::SCREEN_WIDTH, DisplayConfig::SCREEN_HEIGHT}), "Noop",
                                   sf::Style::Close, sf::State::Windowed);
    window.setFramerateLimit(stateMachine.getOsConfig().refreshRate);
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
    // Create a shader for CRT effect
    sf::Shader crtShader;
    if (!crtShader.loadFromFile("src/ui/shaders/my_crt_shader.frag", sf::Shader::Type::Fragment)) {
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

        // Control the update frequency (once every 0.1 seconds)
        // if (loopClock.getElapsedTime().asSeconds() >= 0.001f) {
        // loopClock.restart();  // Reset the clock to track the next 0.1s period
        renderTexture.clear(Colors::WHITE);

        // Draw everything to the render texture (pass renderTexture instead of window)
        toolbar.draw();
        footer.draw();
        main_window.draw(apps, stateMachine.getActiveTab());

        // Apply any post-processing to the render texture (CRT effect)
        renderTexture.display(); // Finalize render texture

        // Control flicker with a random chance (1 in 100)
        float flickerFactor = 0.0f; // Default is no flicker
        if (rand() % 1000 == 0) {
            // 1 in 100 chance to trigger flicker
            flickerFactor = 1.0f;
        }
        // Send parameters to shader
        crtShader.setUniform("flickerFactor", flickerFactor);

        // Set adjustable parameters
        crtShader.setUniform("CRT_CURVE_AMNTx", 0.05f); // Adjust curvature on x-axis
        crtShader.setUniform("CRT_CURVE_AMNTy", 0.05f); // Adjust curvature on y-axis
        crtShader.setUniform("scanLineMultiplier", 1250.0f); // Set the scanline multiplier (original SCAN_LINE_MULT)
        crtShader.setUniform("colorMultiplier", sf::Glsl::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
        // Color multiplier (use to adjust tint)


        // Clear the window and draw the final image
        window.clear(Colors::WHITE);
        window.draw(shaderSprite, &crtShader); // Render the textured sprite with CRT effect
        window.display();

        // }
    }

    return 0;
}
