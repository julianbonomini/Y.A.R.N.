#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <SFML/Graphics.hpp>

#include "../../apps/common/app.hpp"
#include "../../core/state_machine.hpp"

class MainWindow {
public:
    MainWindow(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine);

    void draw(const std::vector<std::unique_ptr<App>> &apps);

private:
    sf::RenderWindow &window;
    const sf::Font &font;
    StateMachine stateMachine;
    void drawTabs(const std::vector<std::unique_ptr<App>> &apps);
    void drawAppArea();

    void drawActiveApp(App * app);
};


#endif //MAIN_WINDOW_H
