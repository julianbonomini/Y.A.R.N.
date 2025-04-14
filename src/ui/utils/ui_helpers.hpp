
#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <SFML/Graphics.hpp>

class UIHelpers {

  public:
    static sf::FloatRect snapToGrid(sf::FloatRect origin);

    static sf::Vector2f snapToGrid(sf::Vector2f origin);

    static std::string formatFloat(float value, int precision);
};



#endif //UI_HELPERS_H
