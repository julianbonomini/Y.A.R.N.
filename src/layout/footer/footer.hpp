#ifndef FOOTER_H
#define FOOTER_H

#include <SFML/Graphics.hpp>

class Footer {
public:
    Footer(sf::RenderTarget &renderer, const sf::Font &font);

    void draw();

private:
    sf::RenderTarget &renderer; // Reference to the SFML render window
    const sf::Font &font; // Reference to the font
};

#endif //FOOTER_H
