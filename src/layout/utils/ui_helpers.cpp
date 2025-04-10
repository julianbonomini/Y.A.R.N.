#include "ui_helpers.hpp"


sf::FloatRect UIHelpers::snapToGrid(sf::FloatRect origin) {
    return sf::FloatRect(
        {std::round(origin.position.x), std::round(origin.position.y)},
        {std::round(origin.size.x), std::round(origin.size.y)}
    );
}

sf::Vector2f UIHelpers::snapToGrid(sf::Vector2f origin) {
    return{std::round(origin.x), std::round(origin.y)};
}
