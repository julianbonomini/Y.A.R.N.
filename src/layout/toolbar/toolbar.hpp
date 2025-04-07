#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <SFML/Graphics.hpp>

class Toolbar {
public:
    Toolbar(sf::RenderWindow &window, const sf::Font &font);

    void draw();

private:
    sf::RenderWindow &window; // Reference to the SFML render window
    const sf::Font &font; // Reference to the font
};

#endif //TOOLBAR_H
