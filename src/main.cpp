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
    float spawnTimer = 0.0f;

    while (window.isOpen()) {
        const float deltaTime = clock.restart().asSeconds();

        processEvents(window, emitter);

        if (emitter.m_isCasting) {
            spawnTimer = spawnTimer + deltaTime;
            if (spawnTimer > config::PARTICLE_SPAWN_TIME) {
                std::cout << spawnTimer << std::endl;
                sf::Vector2f particleSpeed = {config::PARTICLE_INIT_SPEED, 0.0f};
                smokeParticles.emplace_back(emitter.getPosition(), particleSpeed);
                std::cout << "num de particulas = " << smokeParticles.size() << std::endl;
                spawnTimer = spawnTimer - config::PARTICLE_SPAWN_TIME;
            }
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
