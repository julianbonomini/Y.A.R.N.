#include "toolbar.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include "../../globals/theme.hpp"
#include "../../ui/utils/ui_helpers.hpp"

Toolbar::Toolbar(sf::RenderTarget &renderer, const sf::Font &font)
    : renderer(renderer), font(font) {
}

void Toolbar::draw() {
    // Position for OS name (top-left)
    float textX = Layout::PADDING;
    float textY = Layout::PADDING;

    sf::Text osName(font, "NOOP");
    osName.setCharacterSize(FontSizes::TOOLBAR);
    osName.setFillColor(Colors::PRIMARY);
    sf::Vector2f osNamePos = UIHelpers::snapToGrid({textX, textY});
    osName.setPosition(osNamePos);
    renderer.draw(osName);

    // Date/time (top-right)
    time_t now = std::time(nullptr);
    char timeStr[64];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    sf::Text dateAndTime(font, timeStr);
    dateAndTime.setCharacterSize(FontSizes::TOOLBAR);
    dateAndTime.setFillColor(Colors::PRIMARY);

    float rightX = DisplayConfig::SCREEN_WIDTH - dateAndTime.getLocalBounds().size.x - Layout::PADDING;
    sf::Vector2f dateTimePos = UIHelpers::snapToGrid({rightX, textY});
    dateAndTime.setPosition(dateTimePos);
    renderer.draw(dateAndTime);

    // Line separator below toolbar
    sf::RectangleShape line(sf::Vector2f(DisplayConfig::SCREEN_WIDTH, LineStyles::LINE_THICKNESS));
    line.setFillColor(Colors::SECONDARY);
    line.setPosition({0.f, Layout::TOOLBAR_HEIGHT});
    renderer.draw(line);
}
