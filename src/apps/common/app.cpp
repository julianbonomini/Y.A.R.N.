#include "app.hpp"
#include <SFML/Graphics.hpp>


sf::FloatRect App::getGridBox(float colStart, float rowStart, float colSpan = 1, float rowSpan = 1) {
    const sf::Vector2f anchor = TOP_LEFT_ANCHOR;

    float x = anchor.x + colStart * (AppGridLayout::CELL_WIDTH + Layout::PADDING);
    float y = anchor.y + rowStart * (AppGridLayout::CELL_HEIGHT + Layout::PADDING);
    float width = AppGridLayout::CELL_WIDTH * colSpan + Layout::PADDING * (colSpan - 1);
    float height = AppGridLayout::CELL_HEIGHT * rowSpan + Layout::PADDING * (rowSpan - 1);

    return sf::FloatRect({x, y}, {width, height});
}

void App::drawModalRectangle(std::string modalTitle){
    sf::RectangleShape modalRectangle({Layout::MODAL_WIDTH, Layout::MODAL_HEIGHT});
    modalRectangle.setPosition({TOP_LEFT_MODAL_ANCHOR.x, TOP_LEFT_MODAL_ANCHOR.y});
    modalRectangle.setFillColor(Colors::WHITE);
    modalRectangle.setOutlineColor(Colors::GRAY);
    modalRectangle.setOutlineThickness(LineStyles::LINE_THICKNESS);
    window.draw(modalRectangle);

    sf::Text title(font, appName + " " + modalTitle);
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(Colors::BLACK);
    title.setPosition({TOP_LEFT_MODAL_ANCHOR.x + Layout::PADDING, TOP_LEFT_MODAL_ANCHOR.y + Layout::PADDING});
    window.draw(title);
}
