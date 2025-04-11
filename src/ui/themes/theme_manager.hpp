#ifndef THEME_MANAGER_HPP
#define THEME_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Theme {
public:
    virtual ~Theme() = default;
    virtual sf::Color background() const = 0;
    virtual sf::Color primary() const = 0;
    virtual sf::Color secondary() const = 0;
    virtual sf::Color accent() const = 0;
};

class LightTheme : public Theme {
public:
    sf::Color background() const override { return sf::Color::White; }
    sf::Color primary() const override { return sf::Color::Black; }
    sf::Color secondary() const override { return sf::Color(200, 200, 200); }
    sf::Color accent() const override { return sf::Color::Red; }
};

class DarkTheme : public Theme {
public:
    sf::Color background() const override { return sf::Color::Black; }
    sf::Color primary() const override { return sf::Color::White; }
    sf::Color secondary() const override { return sf::Color(80, 80, 80); }
    sf::Color accent() const override { return sf::Color::Yellow; }
};

class ThemeManager {
public:
    static ThemeManager& instance();

    void setTheme(const std::string& themeName);
    const Theme& getCurrentTheme() const;

private:
    ThemeManager();
    ~ThemeManager() = default; // Optional, but good practice for Singletons

    std::map<std::string, std::unique_ptr<Theme>> themes_;
    Theme* currentTheme_;
};

#endif // THEME_MANAGER_HPP