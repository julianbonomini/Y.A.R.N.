#include <iostream>  // Include the input/output library
#include <SFML/Graphics.hpp>  // Include the SFML graphics module

int main() {
    std::cout << "Booting..." << std::endl;

    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}