#include "theme_manager.hpp"

ThemeManager &ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager() : currentTheme_(std::make_unique<LightTheme>().get()) {
    themes_["light"] = std::make_unique<LightTheme>();
    themes_["dark"] = std::make_unique<DarkTheme>();
}

void ThemeManager::setTheme(const std::string &themeName) {
    auto it = themes_.find(themeName);
    if (it != themes_.end()) {
        currentTheme_ = it->second.get();
    }
}

const Theme &ThemeManager::getCurrentTheme() const {
    return *currentTheme_;
}
