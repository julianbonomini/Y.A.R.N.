#ifndef THEME_H
#define THEME_H

#pragma once
#include <SFML/Graphics/Color.hpp>

namespace DisplayConfig {
    constexpr int SCREEN_WIDTH = 800; // EPAPER
    constexpr int SCREEN_HEIGHT = 480; // EPAPER
    // constexpr int SCREEN_WIDTH = 1024; // LCD
    // constexpr int SCREEN_HEIGHT = 600; // LCD
    // constexpr int SCREEN_WIDTH = 1920;
    // constexpr int SCREEN_HEIGHT = 1280;
}

namespace Layout {
    constexpr float WINDOW_WIDTH = DisplayConfig::SCREEN_WIDTH;
    constexpr float WINDOW_HEIGHT = DisplayConfig::SCREEN_HEIGHT;
    constexpr float PADDING = 8.f;
    constexpr float TAB_WIDTH = 120.f;
    constexpr float TAB_HEIGHT = 30.f;
    constexpr float FOOTER_HEIGHT = WINDOW_HEIGHT * 0.08 > 50.f ? 50.f : WINDOW_HEIGHT * 0.08;
    constexpr float TOOLBAR_HEIGHT = 32.f;
    constexpr float TOOLBAR_OFFSET = 40.f + PADDING;
    constexpr float MAIN_APP_WIDTH = WINDOW_WIDTH - 2 * PADDING;
    constexpr float MAIN_APP_HEIGHT = WINDOW_HEIGHT - TOOLBAR_OFFSET - TAB_HEIGHT - 2 * PADDING - FOOTER_HEIGHT;
    constexpr float MODAL_WIDTH = WINDOW_WIDTH; // modal has to cover the entire screen
    constexpr float MODAL_HEIGHT = WINDOW_HEIGHT - TOOLBAR_HEIGHT - FOOTER_HEIGHT; // modal has to cover the entire screen
    constexpr float TEXT_SPACING = 25.f;
    constexpr float LABEL_VALUE_SPACE = 250.f;
}

namespace AppGridLayout {
    constexpr int GRID_COLS = 5;
    constexpr int GRID_ROWS = 4;

    constexpr float CELL_WIDTH = (Layout::MAIN_APP_WIDTH - (GRID_COLS + 1) * Layout::PADDING) / GRID_COLS;
    constexpr float CELL_HEIGHT = (Layout::MAIN_APP_HEIGHT - (GRID_ROWS + 1) * Layout::PADDING) / GRID_ROWS;
}

namespace FontSizes {
    constexpr int TOOLBAR = 12;
    constexpr int FOOTER = 12;
    constexpr int TITLE = 15;
    constexpr int TAB = 15;
    constexpr int DESCRIPTION = 8;
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

namespace LineStyles {
    constexpr float BOX_LINE_THICKNESS = 2.f;
    constexpr float LINE_THICKNESS = 2.f;
}


#endif //THEME_H
