//
// Created by Julian Bonomini on 06/04/2025.
//

#ifndef THEME_H
#define THEME_H

#pragma once
#include <SFML/Graphics/Color.hpp>

namespace Areas {
    constexpr float WINDOW_WIDTH = 800.f;
    constexpr float WINDOW_HEIGHT = 480.f;
    constexpr float PADDING = 8.f;
    constexpr float TAB_WIDTH = 120.f;
    constexpr float TAB_HEIGHT = 30.f;
    constexpr float FOOTER_HEIGHT = 30.f;
    constexpr float TOOLBAR_OFFSET = 40.f + PADDING;
    constexpr float MAIN_APP_WIDTH = WINDOW_WIDTH - 2 * PADDING;
    constexpr float MAIN_APP_HEIGHT = WINDOW_HEIGHT - TOOLBAR_OFFSET - TAB_HEIGHT - 2 * PADDING - FOOTER_HEIGHT;
    constexpr float TEXT_SPACING = 25.f;
    constexpr float LABEL_VALUE_SPACE = 250.f;
}

namespace Grid {
    constexpr int GRID_COLS = 5;
    constexpr int GRID_ROWS = 4;

    constexpr float CELL_WIDTH = (Areas::MAIN_APP_WIDTH - (GRID_COLS + 1) * Areas::PADDING) / GRID_COLS;
    constexpr float CELL_HEIGHT = (Areas::MAIN_APP_HEIGHT - (GRID_ROWS + 1) * Areas::PADDING) / GRID_ROWS;
}

namespace TextSizes {
    constexpr int TITLE = 15;
    constexpr int TAB = 15;
    constexpr int DESCRITPION = 8;
    constexpr int LABEL = 15;
    constexpr int VALUE = 15;
    constexpr int HELP = 10;
}

namespace Colors {
    constexpr sf::Color WHITE = sf::Color::White;
    constexpr sf::Color GRAY = sf::Color(200, 200, 200);
    constexpr sf::Color BLACK = sf::Color::Black;
    constexpr sf::Color DEBUG = sf::Color::Red;
}

namespace Lines {
    constexpr float BOX_LINE_THICKNESS = 2.f;
    constexpr float LINE_THICKNESS = 2.f;
}


#endif //THEME_H
