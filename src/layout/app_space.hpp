#ifndef APP_SPACE_H
#define APP_SPACE_H

#include <SFML/Graphics.hpp>

#include "../apps/app.hpp"

class AppSpace {
public:
    static void draw(sf::RenderWindow& window, const sf::Font& font, const std::vector<std::unique_ptr<App>> &apps, int activeTab);

private:
    static void drawTabs(sf::RenderWindow &window, const sf::Font &font, const std::vector<std::unique_ptr<App>> &apps, int activeTab);
    static void drawAppArea(sf::RenderWindow& window, const std::vector<std::unique_ptr<App>> &apps, int activeTab);
};


#endif //APP_SPACE_H
