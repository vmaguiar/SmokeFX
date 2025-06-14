#include <iostream>
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

    sf::Clock clock;

    while (window.isOpen()) {
        processEvents(window, emitter);

        const float deltaTime = clock.restart().asSeconds();

        if (emitter.m_isCasting) {
            sf::Vector2f particleSpeed = {100.0f, 0.0f};
            smokeParticles.emplace_back(emitter.getPosition(), particleSpeed);
            std::cout << "num de particulas = " << smokeParticles.size() << std::endl;
        }
        for (int i = 0; i < smokeParticles.size();) {
            smokeParticles[i].updateParticle(deltaTime);
            if (smokeParticles[i].getPosition().x > config::WINDOW_SIZE.x) {
                smokeParticles.erase(smokeParticles.begin() + i);
                // smokeParticles[i].setPosition({config::WINDOW_SIZE.x, smokeParticles[i].getPosition().y});
                // smokeParticles[i].setVelocity({(-1 * smokeParticles[i].getVelocity().x), smokeParticles[i].getVelocity().y});
            }
            else if (smokeParticles[i].getPosition().x < 0) {
                smokeParticles.erase(smokeParticles.begin() + i);
            }
            else if (smokeParticles[i].getPosition().y > config::WINDOW_SIZE.y) {
                smokeParticles.erase(smokeParticles.begin() + i);
                // smokeParticles[i].setPosition({smokeParticles[i].getPosition().x, config::WINDOW_SIZE.y});
                // smokeParticles[i].setVelocity({smokeParticles[i].getVelocity().x, (-1 * smokeParticles[i].getVelocity().y)});
            }
            else if (smokeParticles[i].getPosition().y < 0) {
                smokeParticles.erase(smokeParticles.begin() + i);
            }
            else {
                i++;
            }
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
