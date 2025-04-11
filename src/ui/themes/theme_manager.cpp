#include "theme_manager.hpp"
#include <memory> // Still good to ensure this is included

ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager() {
    themes_["dark"] = std::make_unique<DarkTheme>();
    themes_["bios"] = std::make_unique<BiosTheme>();
    themes_["eink"] = std::make_unique<eInkTheme>();
    currentTheme_ = themes_["eink"].get(); // Initialize with a valid Theme
}

void ThemeManager::setTheme(const std::string& themeName) {
    auto it = themes_.find(themeName);
    if (it != themes_.end()) {
        currentTheme_ = it->second.get();
    }
}

const Theme& ThemeManager::getCurrentTheme() const {
    return *currentTheme_;
}