#include <iostream>
#include <thread>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <thread>
#include <atomic>
#include <mutex>

#include "apps/config/config_app.hpp"
#include "layout/toolbar/toolbar.hpp"
#include "layout/footer/footer.hpp"
#include "globals/ui_globals.hpp"
#include "apps/info/info_app.hpp"
#include "apps/market/market_app.hpp"
#include "apps/pomodoro/pomodoro_app.hpp"
#include "apps/weather/openweather.hpp"
#include "apps/weather/weather_app.hpp"
#include "apps/market/market_daemon_client.hpp"
#include "layout/main_window/main_window.hpp"
#include "core/state_machine/state_machine.hpp"
#include "ui/themes/theme_manager.hpp"
#include "common/logger.hpp"
#include "core/env/config.hpp"
#include "core/execute/execute_utils.hpp"
#include "core/state_machine/market_state.hpp"
#include "core/state_machine/pomodoro_state.hpp"


void drawSplashScreen(
    sf::RenderWindow &window,
    sf::Font &font,
    sf::RenderTexture &renderTexture,
    sf::Sprite &shaderSprite,
    sf::Shader &crtShader,
    bool &shownSplash
) {
    renderTexture.clear(sf::Color(50, 50, 50));

    sf::Text splash(font, "Y.A.R.N.");
    splash.setFillColor(sf::Color::White);
    splash.setCharacterSize(100);
    // Center the text
    sf::FloatRect textRect = splash.getLocalBounds();
    splash.setOrigin({textRect.size.x / 2, textRect.size.y / 2});
    splash.setPosition({DisplayConfig::SCREEN_WIDTH / 2, DisplayConfig::SCREEN_HEIGHT / 2 - textRect.size.y});
    renderTexture.draw(splash, &crtShader);


    sf::Text subTitle(font, "( Yet Another Ridiculous Name )");
    subTitle.setFillColor(sf::Color::White);
    subTitle.setCharacterSize(25);
    sf::FloatRect subTitleRec = subTitle.getLocalBounds();
    subTitle.setOrigin({subTitleRec.size.x / 2, subTitleRec.size.y / 2});
    subTitle.setPosition({DisplayConfig::SCREEN_WIDTH / 2, DisplayConfig::SCREEN_HEIGHT / 2 + textRect.size.y});
    renderTexture.draw(subTitle, &crtShader);

    renderTexture.display();

    window.draw(shaderSprite, &crtShader);

    window.display();
    shownSplash = true;

    sleep(sf::seconds(1));
}

pid_t marketDaemonPid = -1;

void killMarketDaemon() {
    Logger::info("Killing Python daemon with PID:", marketDaemonPid);
    if (marketDaemonPid > 0) {
        kill(marketDaemonPid, SIGTERM);
        waitpid(marketDaemonPid, nullptr, 0); // Wait for it to clean up
    }
}

void signalHandler(int signum) {
    Logger::warning("C++ app received signal", signum, ", cleaning up...");
    if (marketDaemonPid > 0) {
        killMarketDaemon();
    }
    exit(signum);
}

bool waitForDaemonStartup(int maxTries = 10, int delayMs = 1000) {
    for (int i = 0; i < maxTries; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        bool isReady = MarketDaemonClient::ready();
        if (isReady) {
            return true;
        }
        Logger::debug("deamon not ready yet");
    }
    return false;
}

pid_t startMarketDaemon(const std::string &daemonPath, std::vector<std::string> &symbols, std::vector<std::string> &trackers) {
    pid_t pid = fork();
    if (pid == 0) {
        std::vector<const char *> args;
        args.push_back("python3"); // argv[0]
        args.push_back(daemonPath.c_str());
        args.push_back("--symbols");
        for (const auto &symbol: symbols) {
            args.push_back(symbol.c_str());
        }
        args.push_back("--trackers");
        for (const auto &tracker: trackers) {
            args.push_back(tracker.c_str());
        }
        args.push_back(nullptr); // Null-terminated

        // Execute the daemon
        execvp("python3", const_cast<char * const*>(args.data()));
        // In child: replace process with the Python daemon
        // execlp("python3", "python3", daemonPath.c_str(), symbols, nullptr);

        // If execlp fails:
        Logger::error("Market daemon fork failed");
        perror("Failed to start Python daemon");
        exit(1);
    } else if (pid > 0) {
        Logger::info("Market daemon started with PID:", pid);
        marketDaemonPid = pid;
        return pid;
    } else {
        Logger::error("Market daemon fork failed");
        perror("fork failed");
        return -1;
    }
}

int main() {
    Logger::info("Booting...");
    // Register signal handler
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    Logger::done_separator();

    Logger::info("Loading env config...");
    std::unordered_map<std::string, std::string> envConfig = EnvConfig::load_env_config();
    Logger::done_separator();

    // Create the StateMachine instance to manage app state.
    Logger::info("Initializing state machines from disk...");
    StateMachine stateMachine(Tab::INF, envConfig);
    OsConfigFile *os_config_file = &stateMachine.getOsConfig(); // by ref, don't copy
    OsConfigFile initial_os_config_file = stateMachine.getOsConfig(); // by ref, don't copy
    Logger::done_separator();

    Logger::info("Initializing app states...");
    WeatherState weatherState({});
    MarketState marketState({});
    PomodoroState pomodoroState;
    Logger::done_separator();

    auto window = sf::RenderWindow(sf::VideoMode({DisplayConfig::SCREEN_WIDTH, DisplayConfig::SCREEN_HEIGHT}),
                                   "Y.A.R.N",
                                   sf::Style::Default, sf::State::Windowed);

    window.setFramerateLimit(initial_os_config_file.refreshRate);
    int loadedRefreshRate = initial_os_config_file.refreshRate; // Copies the value

    sf::Font font;
    if (!font.openFromFile(ExecuteUtils::getResourcePath("assets/fonts/PxPlus_IBM_VGA8.ttf"))) {
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
    shaderFile << ExecuteUtils::getResourcePath("assets/shaders/") << initial_os_config_file.shader << ".frag";
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


    Logger::info("Starting forked market data daemon...");
    // Start the Python daemon in a separate thread
    std::string daemonPath = ExecuteUtils::getResourcePath("assets/daemons/market_daemon.py");
    marketDaemonPid = startMarketDaemon(daemonPath, stateMachine.getMarketConfig().symbols, stateMachine.getMarketConfig().trackers);
    waitForDaemonStartup();
    bool isMarketOpen = MarketDaemonClient::isMarketOpen();
    marketState.updateMarketOpen(isMarketOpen);
    Logger::done_separator();


    Logger::info("Starting app clocks...");
    // Market clock
    Logger::debug("Starting market data clock with interval...", stateMachine.getMarketConfig().refreshIntervalInMinutes, "minutes");
    sf::Clock marketClock;
    marketClock.start();
    const sf::Time marketInterval = sf::seconds(stateMachine.getMarketConfig().refreshIntervalInMinutes * 60);
    // Weather Clock
    Logger::debug("Setting weather data clock with interval...", stateMachine.getWeatherConfig().refreshIntervalInMinutes, "minutes");
    sf::Clock weatherClock;
    weatherClock.start();
    const sf::Time weatherInterval = sf::seconds(stateMachine.getWeatherConfig().refreshIntervalInMinutes * 60);
    // Tab cycling Clock
    Logger::debug("Setting tab cycling clock with interval...", stateMachine.getOsConfig().cycleTabTimeInSeconds, "seconds");
    sf::Clock tabCycleClock;
    tabCycleClock.start();
    const sf::Time tabCycleInterval = sf::seconds(stateMachine.getOsConfig().cycleTabTimeInSeconds);
    Logger::done_separator();

    Logger::info("Initializing apps...");
    std::vector<std::unique_ptr<App> > apps;
    // These are ordered based on the enum on tabs.hpp
    for (int i = 0; i < static_cast<int>(Tab::COUNT); ++i) {
        Tab tab = static_cast<Tab>(i);
        Logger::debug("Initializing", Tabs::tabToString(tab), "app...");
        switch (tab) {
            case Tab::MKT:
                apps.push_back(std::make_unique<MarketApp>("MKT", renderTexture, font, stateMachine, marketState));
                break;
            case Tab::PMD:
                apps.push_back(std::make_unique<PomodoroApp>("PMD", renderTexture, font, stateMachine, pomodoroState));
                break;
            case Tab::WTH:
                apps.push_back(std::make_unique<WeatherApp>("WTH", renderTexture, font, stateMachine, weatherState));
                break;
            case Tab::INF:
                apps.push_back(std::make_unique<InfoApp>("INF", renderTexture, font));
                break;
            case Tab::CNF:
                apps.push_back(std::make_unique<ConfigApp>("CNF", renderTexture, font, stateMachine));
                break;
            default: throw std::runtime_error("Invalid tab");
        }
    }
    assert(apps.size() == static_cast<size_t>(Tab::COUNT));
    Logger::info("Apps booted successfully...");
    Logger::done_separator();

    bool initalMarketDataLoaded = false;
    bool initalWeatherDataLoaded = false;
    bool shownSplash = false;
    sf::Vector2f lastMouseClickPos;
    bool mouseClicked = false;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (auto mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                lastMouseClickPos = window.mapPixelToCoords({mousePress->position.x, mousePress->position.y});
                mouseClicked = true;
                Logger::debug("EVENT:", "Mouse clicked", mousePress->position.x, mousePress->position.y);
            }

            if (event->is<sf::Event::Closed>()) {
                Logger::info("Window closed, cleaning up services and finalising app.");
                killMarketDaemon();
                window.close();
            }
            // Global keys (tab switching)
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                auto activeApp = apps[Tabs::tabToIndex(stateMachine.getActiveTab())].get();
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
                        stateMachine.previousTab();
                        break;
                    }

                    // Tab right
                    if (keyPressed->scancode == sf::Keyboard::Scan::S) {
                        stateMachine.nextTab();
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

                    apps[Tabs::tabToIndex(stateMachine.getActiveTab())]->handleEvent(*keyPressed);
                }
            }
        }

        if (!shownSplash) {
            drawSplashScreen(window, font, renderTexture, shaderSprite, crtShader, shownSplash);
        }
        // Check if global settings have changed:
        if (os_config_file->shader != loadaedShader && loadaedShader != "") {
            Logger::info("Changing shader to", os_config_file->shader, "...");
            std::ostringstream newShader;
            newShader << ExecuteUtils::getResourcePath("assets/shaders/") << os_config_file->shader << ".frag";
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

        // Init weather data
        if (!initalWeatherDataLoaded) {
            Logger::info("Init weather data ...");
            nlohmann::json weatherData = OpenWeather::getWeather(stateMachine.getWeatherConfig().city);
            weatherState.updateFromJson(weatherData);
            initalWeatherDataLoaded = true;
            Logger::done_separator();
        }

        // Refersh weather data
        if (weatherClock.getElapsedTime() >= weatherInterval) {
            Logger::info("Updating weather ...");
            nlohmann::json weatherData = OpenWeather::getWeather(stateMachine.getWeatherConfig().city);
            weatherState.updateFromJson(weatherData);
            weatherClock.restart();
            Logger::done_separator();
        }

        // Cycle active tab if enabled
        if (stateMachine.getOsConfig().cycleTabsEnabled && tabCycleClock.getElapsedTime() >= tabCycleInterval) {
            Logger::info("Cycling active tab ...");
            stateMachine.nextTab();
            tabCycleClock.restart();
            Logger::done_separator();
        }

        // Update pomodoro clock if running
        PomodoroState::updatePomodoroClockIfRunning(pomodoroState, stateMachine);

        // Clear screen with base color
        renderTexture.clear(ThemeManager::instance().getCurrentTheme().background());
        // Draw everything
        toolbar.draw();
        footer.draw();
        main_window.draw(apps, stateMachine, mouseClicked, lastMouseClickPos);
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

        // Market data can be quite slow, for this reason there is a mocked init until data is available

        // Init market data once
        if (!initalMarketDataLoaded) {
            Logger::info("Init stock quotes async ...");
            // Only trigger a fetch if one isn't already in progress
            static std::atomic<bool> fetching = false;
            if (!fetching) {
                fetching = true;
                std::thread([] {
                    MarketDaemonClient::fetchAllQuotesAsync(&fetching);
                }).detach();
            }
            Logger::info("Getting stock quotes ...");
            initalMarketDataLoaded = true;
            Logger::done_separator();
        }

        // Refresh market data
        if (marketClock.getElapsedTime() >= marketInterval) {
            Logger::info("Fetching stock quotes async ...");
            bool isMarketOpen = MarketDaemonClient::isMarketOpen();
            if (isMarketOpen != marketState.getIsMarketOpen()) {
                marketState.updateMarketOpen(isMarketOpen);
            }
            if (isMarketOpen) {
                // Only trigger a fetch if one isn't already in progress
                static std::atomic<bool> fetching = false;
                if (!fetching) {
                    fetching = true;
                    std::thread([] {
                        MarketDaemonClient::fetchAllQuotesAsync(&fetching);
                    }).detach();
                }
                Logger::info("Getting stock quotes ...");
            } else {
                Logger::debug("Market closed, not fetching anything");
            }
            marketClock.restart();
            Logger::done_separator();
        }
        // Apply new quotes if available
        if (MarketDaemonClient::areQuotesReady()) {
            Logger::debug("Updating market state with async quotes");
            auto quotes = MarketDaemonClient::getLatestQuotes();
            marketState.updateAllQuotes(quotes);
            MarketDaemonClient::setQuotesReadyToFalse();
        }
    }

    return 0;
}
