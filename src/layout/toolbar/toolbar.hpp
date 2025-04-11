#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <SFML/Graphics.hpp>

class Toolbar {
public:
    Toolbar(sf::RenderTarget &renderer, const sf::Font &font);

    void draw();

private:
    sf::RenderTarget &renderer;
    const sf::Font &font;
};

#endif //TOOLBAR_H
