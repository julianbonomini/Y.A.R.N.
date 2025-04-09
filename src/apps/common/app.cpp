#include "app.hpp"
#include <SFML/Graphics.hpp>


sf::FloatRect App::getGridBox(int colStart, int rowStart, int colSpan = 1, int rowSpan = 1) {
    const sf::Vector2f anchor = TOP_LEFT_ANCHOR;

    float x = anchor.x + colStart * (Grid::CELL_WIDTH + Areas::PADDING);
    float y = anchor.y + rowStart * (Grid::CELL_HEIGHT + Areas::PADDING);
    float width = Grid::CELL_WIDTH * colSpan + Areas::PADDING * (colSpan - 1);
    float height = Grid::CELL_HEIGHT * rowSpan + Areas::PADDING * (rowSpan - 1);

    return sf::FloatRect({x, y}, {width, height});
}

void App::drawModalRectangle(sf::RenderWindow &window, std::string modalTitle){
    sf::RectangleShape modalRectangle({Areas::MODAL_WIDTH, Areas::MODAL_HEIGHT});
    modalRectangle.setPosition({TOP_LEFT_MODAL_ANCHOR.x, TOP_LEFT_MODAL_ANCHOR.y});
    modalRectangle.setFillColor(Colors::WHITE);
    modalRectangle.setOutlineColor(Colors::GRAY);
    modalRectangle.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(modalRectangle);

    sf::Text title(font, appName + " " + modalTitle);
    title.setCharacterSize(TextSizes::TITLE);
    title.setFillColor(Colors::BLACK);
    title.setPosition({TOP_LEFT_MODAL_ANCHOR.x + Areas::PADDING, TOP_LEFT_MODAL_ANCHOR.y + Areas::PADDING});
    window.draw(title);
}
