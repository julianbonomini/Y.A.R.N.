#include "footer.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include "../../globals/theme.hpp"

Footer::Footer(sf::RenderWindow &window, const sf::Font &font)
    : window(window), font(font) {
}


void Footer::draw() {
    float textX = Layout::PADDING * 2;
    float textY = Layout::WINDOW_HEIGHT - Layout::FOOTER_HEIGHT + Layout::PADDING;

    sf::Text move(font, "<A|S> TAB_SWITCH", FontSizes::FOOTER);
    move.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    move.setPosition({textX, textY});
    window.draw(move);

    sf::Text help(font, "<H> APP_HELP", FontSizes::FOOTER);
    help.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    help.setPosition({move.getGlobalBounds().position.x + move.getGlobalBounds().size.x + Layout::PADDING * 2, textY});
    window.draw(help);

    sf::Text menu(font, "<C> APP_SETTINGS", FontSizes::FOOTER);
    menu.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    menu.setPosition({help.getGlobalBounds().position.x + help.getGlobalBounds().size.x + Layout::PADDING * 2, textY});
    window.draw(menu);

    sf::Text close(font, "<ESC> CLOSE_MODAL", FontSizes::FOOTER);
    close.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    close.setPosition({menu.getGlobalBounds().position.x + menu.getGlobalBounds().size.x + Layout::PADDING * 2, textY});
    window.draw(close);
}
