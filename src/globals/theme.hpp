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
    const float WINDOW_WIDTH = 800.f;
    const float WINDOW_HEIGHT = 480.f;
    const float PADDING = 8.f;
    const float TAB_WIDTH = 120.f;
    const float TAB_HEIGHT = 30.f;
    const float TOOLBAR_OFFSET = 40.f + PADDING;
    const float MAIN_APP_WIDTH = WINDOW_WIDTH - 2 * PADDING;
    const float MAIN_APP_HEIGHT = WINDOW_HEIGHT - TOOLBAR_OFFSET - TAB_HEIGHT - 2 * PADDING;
}


#endif //THEME_H
