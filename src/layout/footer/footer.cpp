#include "footer.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include "../../globals/theme.hpp"

Footer::Footer(sf::RenderWindow &window, const sf::Font &font)
    : window(window), font(font) {
}


void Footer::draw() {
    float textX = Areas::PADDING * 2;
    float textY = Areas::WINDOW_HEIGHT - Areas::FOOTER_HEIGHT + Areas::PADDING;

    sf::Text move(font, "<A|S> TAB_SWITCH", TextSizes::FOOTER);
    move.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    move.setPosition({textX, textY});
    window.draw(move);

    sf::Text help(font, "<H> APP_HELP", TextSizes::FOOTER);
    help.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    help.setPosition({move.getGlobalBounds().position.x + move.getGlobalBounds().size.x + Areas::PADDING * 2, textY});
    window.draw(help);

    sf::Text menu(font, "<C> APP_SETTINGS", TextSizes::FOOTER);
    menu.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    menu.setPosition({help.getGlobalBounds().position.x + help.getGlobalBounds().size.x + Areas::PADDING * 2, textY});
    window.draw(menu);

    sf::Text close(font, "<ESC> CLOSE_MODAL", TextSizes::FOOTER);
    close.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    close.setPosition({menu.getGlobalBounds().position.x + menu.getGlobalBounds().size.x + Areas::PADDING * 2, textY});
    window.draw(close);
}
