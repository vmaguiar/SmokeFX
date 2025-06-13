#include <SFML/Graphics.hpp>

#include "configConsts.hpp"
#include "events.hpp"

int main() {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(config::windowSize), "SmokeFX Workss!");
    window.setFramerateLimit(config::maxFramerate);

    while (window.isOpen()) {
        processEvents(window);

        window.clear();
        // Render...
        sf::CircleShape circle(30);
        window.draw(circle);
        window.display();
    }
}
