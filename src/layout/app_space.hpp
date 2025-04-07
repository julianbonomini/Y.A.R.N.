#ifndef APP_SPACE_H
#define APP_SPACE_H

#include <SFML/Graphics.hpp>

class AppSpace {
public:
    static void draw(sf::RenderWindow& window, const sf::Font& font, const std::vector<std::string>& apps, int activeTab);

    static void drawTabs(sf::RenderWindow &window, const sf::Font &font, const std::vector<std::string> &apps, int activeTab);

    static void drawAppArea(sf::RenderWindow &window);
};

#endif //APP_SPACE_H
