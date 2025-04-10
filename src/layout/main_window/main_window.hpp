#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <SFML/Graphics.hpp>

#include "../../apps/common/app.hpp"
#include "../../core/state_machine/state_machine.hpp"

class MainWindow {
public:
    MainWindow(sf::RenderWindow &window, const sf::Font &font);

    void draw(const std::vector<std::unique_ptr<App> > &apps, int activeTab);

private:
    sf::RenderWindow &window;
    const sf::Font &font;

    void drawTabs(const std::vector<std::unique_ptr<App> > &apps, int activeTab);

    void drawAppArea();

    void drawActiveApp(App *app);

    void drawFooter();
};


#endif //MAIN_WINDOW_H
