#include "ui_helpers.hpp"
#include<sstream>

#ifdef __linux__
#include<cmath>
#endif

sf::FloatRect UIHelpers::snapToGrid(sf::FloatRect origin) {
    return sf::FloatRect(
        {std::round(origin.position.x), std::round(origin.position.y)},
        {std::round(origin.size.x), std::round(origin.size.y)}
    );
}

sf::Vector2f UIHelpers::snapToGrid(sf::Vector2f origin) {
    return{std::round(origin.x), std::round(origin.y)};
}

std::string UIHelpers::formatFloat(float value, int precision = 2) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

std::string UIHelpers::underscoreSeparator(std::string original) {
    std::replace(original.begin(), original.end(), ' ', '_');
    return original;
}