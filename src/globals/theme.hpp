//
// Created by Julian Bonomini on 06/04/2025.
//

#ifndef THEME_H
#define THEME_H

#pragma once
#include <SFML/Graphics/Color.hpp>

namespace Areas {
    const float WINDOW_WIDTH = 800.f;
    const float WINDOW_HEIGHT = 480.f;
    const float PADDING = 8.f;
    const float TAB_WIDTH = 120.f;
    const float TAB_HEIGHT = 30.f;
    const float HELP_BAR_HEIGHT = 30.f;
    const float TOOLBAR_OFFSET = 40.f + PADDING;
    const float MAIN_APP_WIDTH = WINDOW_WIDTH - 2 * PADDING;
    const float MAIN_APP_HEIGHT = WINDOW_HEIGHT - TOOLBAR_OFFSET - TAB_HEIGHT - 2 * PADDING - HELP_BAR_HEIGHT;
    const float TEXT_SPACING = 25.f;
    const float LABEL_VALUE_SPACE = 250.f;
}

namespace TextSizes {
    const int TITLE = 15;
    const int TAB = 15;
    const int DESCRITPION = 8;
    const int LABEL = 15;
    const int VALUE = 15;
    const int HELP = 10;
}

namespace Colors {
    const sf::Color Background = sf::Color::White;
    const sf::Color DarkerBackground = sf::Color(200, 200, 200);
    const sf::Color Text = sf::Color::Black;
    const sf::Color Accent = sf::Color::Red;
}

namespace Lines {
    const float BOX_LINE_THICKNESS = 2.f;
}


#endif //THEME_H
