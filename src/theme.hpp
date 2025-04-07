//
// Created by Julian Bonomini on 06/04/2025.
//

#ifndef THEME_H
#define THEME_H

#pragma once
#include <SFML/Graphics/Color.hpp>

namespace Theme {
    const sf::Color Background          = sf::Color::White;
    const sf::Color DarkerBackground    = sf::Color(200, 200, 200);
    const sf::Color Text                = sf::Color::Black;
    const sf::Color Accent              = sf::Color::Red;
    const int WINDOW_HEIGHT = 480.f;
    const int WINDOW_WIDTH = 800.f;
}


#endif //THEME_H
