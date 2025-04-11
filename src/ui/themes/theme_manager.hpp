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
    virtual sf::Color highlight() const = 0;
};

class DarkTheme : public Theme {
public:
    sf::Color background() const override { return sf::Color(60, 61, 55); }
    sf::Color primary() const override { return sf::Color::White; }
    sf::Color secondary() const override { return sf::Color(236, 223, 204); }
    sf::Color accent() const override { return sf::Color::Yellow; }
    sf::Color highlight() const override { return sf::Color(105, 117, 101); }
};

class BiosTheme : public Theme {
public:
    sf::Color background() const override { return sf::Color(1, 46, 220); }
    sf::Color primary() const override { return sf::Color::White; }
    sf::Color secondary() const override { return sf::Color(236, 223, 204); }
    sf::Color accent() const override { return sf::Color::Yellow; }
    sf::Color highlight() const override { return sf::Color(210, 18, 39); }
};

class eInkTheme : public Theme {
public:
    sf::Color background() const override { return sf::Color::White; }
    sf::Color primary() const override { return sf::Color::Black; }
    sf::Color secondary() const override { return sf::Color(200, 200, 200); }
    sf::Color accent() const override { return sf::Color(200, 200, 200); }
    sf::Color highlight() const override { return sf::Color(200, 200, 200); }
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