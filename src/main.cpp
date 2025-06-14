#include <SFML/Graphics.hpp>

#include "configConsts.hpp"
#include "events.hpp"
#include "particle.hpp"
#include  "emitter.hpp"

int main() {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(config::WINDOW_SIZE), "SmokeFX Workss!");
    window.setFramerateLimit(config::MAX_FRAMERATE);

    Emitter emitter(config::EMITTER_START_POSITION);
    std::vector<Particle> smokeParticles;

    // sf::Clock clock;

    while (window.isOpen()) {
        processEvents(window, emitter);

        if (emitter.m_isCasting) {
            sf::Vector2f particleSpeed = {100.0f, 0.0f};
            smokeParticles.emplace_back(emitter.getPosition(), particleSpeed);
        }

        window.clear();
        // Render...
        for (Particle &p: smokeParticles) {
            p.draw(window);
        }
        emitter.draw(window);
        window.display();
    }
}
