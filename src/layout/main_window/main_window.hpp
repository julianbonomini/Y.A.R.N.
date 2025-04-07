#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <SFML/Graphics.hpp>

#include "../../apps/common/app.hpp"

class MainWindow {
public:
    MainWindow(sf::RenderWindow &window, const sf::Font &font);

    void draw(const std::vector<std::unique_ptr<App>> &apps, int activeTab);

private:
    sf::RenderWindow& window;  // Reference to the SFML render window
    const sf::Font& font;      // Reference to the font
    void drawTabs(const std::vector<std::unique_ptr<App>> &apps, int activeTab);
    void drawAppArea();

    void drawActiveApp(App * app);
};


#endif //MAIN_WINDOW_H
