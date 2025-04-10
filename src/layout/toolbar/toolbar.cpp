#include "toolbar.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include "../../globals/theme.hpp"
#include "../../ui/utils/ui_helpers.hpp"

Toolbar::Toolbar(sf::RenderWindow &window, const sf::Font &font)
    : window(window), font(font) {
}


void Toolbar::draw() {
    sf::Text osName(font, "NOOP");
    osName.setCharacterSize(FontSizes::TOOLBAR);
    osName.setFillColor(Colors::BLACK);
    osName.setPosition({10.f, 10.f});
    window.draw(osName);

    // Get current date and time
    time_t now = std::time(nullptr);
    char timeStr[64];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    sf::Text dateAndTime(font, timeStr);
    dateAndTime.setCharacterSize(FontSizes::TOOLBAR);
    dateAndTime.setFillColor(Colors::BLACK);
    sf::Vector2f dateAndTimePosition = UIHelpers::snapToGrid({window.getSize().x - 145.f, 10.f});
    dateAndTime.setPosition(dateAndTimePosition);
    window.draw(dateAndTime);

    // Draw a thin horizontal line below the text
    sf::RectangleShape line(sf::Vector2f(window.getSize().x, LineStyles::LINE_THICKNESS));
    line.setFillColor(Colors::GRAY);
    line.setPosition({0.f, Layout::TOOLBAR_HEIGHT});
    window.draw(line);

}
