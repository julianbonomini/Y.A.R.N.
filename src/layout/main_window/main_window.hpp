#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <SFML/Graphics.hpp>

#include "../../apps/common/app.hpp"
#include "../../core/state_machine/state_machine.hpp"

class MainWindow {
public:
    MainWindow(sf::RenderTarget &renderer, const sf::Font &font);

    void draw(std::vector<std::unique_ptr<App>> &apps, StateMachine &stateMachine, bool &mouseClicked, sf::Vector2f &mouseLastPosition);

private:
    sf::RenderTarget &renderer;
    const sf::Font &font;

    void drawTabs(const std::vector<std::unique_ptr<App> > &apps, int activeTab, bool &mouseClicked, sf::Vector2f &mouseLastPosition, StateMachine &stateMachine);

    void drawAppArea();

    void healTab(const std::vector<std::unique_ptr<App>> &apps, StateMachine &stateMachine);

    void drawActiveApp(const std::vector<std::unique_ptr<App> > &apps, StateMachine &stateMachine);

    void drawFooter();
};


#endif //MAIN_WINDOW_H
