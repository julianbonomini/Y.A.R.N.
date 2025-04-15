#include "app.hpp"
#include <SFML/Graphics.hpp>

#include "../../ui/themes/theme_manager.hpp"
#ifdef __linux__
#include<cmath>
#endif

sf::FloatRect App::getGridBox(float colStart, float rowStart, float colSpan = 1, float rowSpan = 1) {
    const sf::Vector2f anchor = TOP_LEFT_ANCHOR;

    float x = anchor.x + colStart * (AppGridLayout::CELL_WIDTH + Layout::PADDING);
    float y = anchor.y + rowStart * (AppGridLayout::CELL_HEIGHT + Layout::PADDING);
    float width = AppGridLayout::CELL_WIDTH * colSpan + Layout::PADDING * (colSpan - 1);
    float height = AppGridLayout::CELL_HEIGHT * rowSpan + Layout::PADDING * (rowSpan - 1);

    return sf::FloatRect(
        {std::round(x), std::round(y)},
        {std::round(width), std::round(height)}
    );
};

sf::FloatRect App::drawModalRectangle(std::string modalTitle) {
    sf::FloatRect box = getGridBox(0, 0, 5, 4);
    sf::RectangleShape modalRectangle({box.size.x, box.size.y});
    modalRectangle.setPosition({box.position.x, box.position.y});
    modalRectangle.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    modalRectangle.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    modalRectangle.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(modalRectangle);

    sf::Text title(font, appName + " " + modalTitle);
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);
    return box;
};

std::string App::wrapText(const std::string &text, float maxWidth, unsigned int characterSize) {
    sf::Text tempText(font);
    tempText.setCharacterSize(characterSize);
    std::string wrappedText;
    std::string currentLine;
    std::istringstream wordStream(text);
    std::string word;

    while (wordStream >> word) {
        tempText.setString(currentLine + (currentLine.empty() ? "" : " ") + word);
        if (tempText.getLocalBounds().size.x > maxWidth) {
            wrappedText += currentLine + "\n";
            currentLine = word;
        } else {
            currentLine += (currentLine.empty() ? "" : " ") + word;
        }
    }

    wrappedText += currentLine;
    return wrappedText;
}
