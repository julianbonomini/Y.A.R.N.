#ifndef APP_SPACE_H
#define APP_SPACE_H

#include <SFML/Graphics.hpp>

class AppSpace {
public:
    static void draw(sf::RenderWindow& window, const sf::Font& font, const std::vector<std::string>& tabs, int activeTab);
};

#endif //APP_SPACE_H
