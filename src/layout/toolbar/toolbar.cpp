#include "toolbar.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include "../../globals/theme.hpp"
#include "../../ui/utils/ui_helpers.hpp"

Toolbar::Toolbar(sf::RenderWindow &window, const sf::Font &font)
    : window(window), font(font) {
}

void Toolbar::draw() {
    // Position for OS name (top-left)
    float textX = Layout::PADDING;
    float textY = Layout::PADDING;

    sf::Text osName(font, "NOOP");
    osName.setCharacterSize(FontSizes::TOOLBAR);
    osName.setFillColor(Colors::BLACK);
    sf::Vector2f osNamePos = UIHelpers::snapToGrid({textX, textY});
    osName.setPosition(osNamePos);
    window.draw(osName);

    // Date/time (top-right)
    time_t now = std::time(nullptr);
    char timeStr[64];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    sf::Text dateAndTime(font, timeStr);
    dateAndTime.setCharacterSize(FontSizes::TOOLBAR);
    dateAndTime.setFillColor(Colors::BLACK);

    float rightX = DisplayConfig::SCREEN_WIDTH - dateAndTime.getLocalBounds().size.x - Layout::PADDING;
    sf::Vector2f dateTimePos = UIHelpers::snapToGrid({rightX, textY});
    dateAndTime.setPosition(dateTimePos);
    window.draw(dateAndTime);

    // Line separator below toolbar
    sf::RectangleShape line(sf::Vector2f(DisplayConfig::SCREEN_WIDTH, LineStyles::LINE_THICKNESS));
    line.setFillColor(Colors::GRAY);
    line.setPosition({0.f, Layout::TOOLBAR_HEIGHT});
    window.draw(line);
}
