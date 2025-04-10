#include "footer.hpp"
#include <SFML/Graphics.hpp>
#include "../../globals/theme.hpp"
#include "../../ui/utils/ui_helpers.hpp"

Footer::Footer(sf::RenderTarget &renderer, const sf::Font &font)
    : renderer(renderer), font(font) {
}


void Footer::draw() {
    float textX = Layout::PADDING * 2;
    float textY = Layout::WINDOW_HEIGHT - Layout::FOOTER_HEIGHT + Layout::PADDING;

    sf::Text move(font, "<A|S> TAB_SWITCH", FontSizes::FOOTER);
    move.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    sf::Vector2f movePos = UIHelpers::snapToGrid({textX, textY});
    move.setPosition(movePos);
    renderer.draw(move);

    sf::Text help(font, "<H> APP_HELP", FontSizes::FOOTER);
    help.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    sf::Vector2f helpPos = UIHelpers::snapToGrid({move.getGlobalBounds().position.x + move.getGlobalBounds().size.x + Layout::PADDING * 2, textY});
    help.setPosition(helpPos);
    renderer.draw(help);

    sf::Text menu(font, "<C> APP_SETTINGS", FontSizes::FOOTER);
    menu.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    sf::Vector2f menuPos = UIHelpers::snapToGrid({help.getGlobalBounds().position.x + help.getGlobalBounds().size.x + Layout::PADDING * 2, textY});
    menu.setPosition(menuPos);
    renderer.draw(menu);

    sf::Text close(font, "<ESC> CLOSE_MODAL", FontSizes::FOOTER);
    close.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    sf::Vector2f closePos = UIHelpers::snapToGrid({menu.getGlobalBounds().position.x + menu.getGlobalBounds().size.x + Layout::PADDING * 2, textY});
    close.setPosition(closePos);
    renderer.draw(close);
}
