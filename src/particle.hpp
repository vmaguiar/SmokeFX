#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

class Particle {
    sf::RectangleShape m_particleShape;
    sf::Vector2f m_particleVelocity;
    sf::Vector2f m_particleAcceleration;

    float m_particleCurrentRotation; // degrees
    float m_particleRotationSpeed; // degrees per seconds

    float m_particleInitialSize;
    float m_particleCurrentSize;
    float m_particleScaleRate; // pixels per seconds ou %/sec

    sf::Color m_particleInitialColor;
    float m_particleCurrentAlpha; // (0-255) empty - full
    float m_particleDecayRate; // alpha per seconds

    std::chrono::high_resolution_clock::time_point m_spawnTime;
    float m_particleLifetime; // total in seconds

public:
    Particle(sf::Vector2f startPosition, sf::Vector2f startVelocity, sf::Color startColor, float startSize, float lifeTime,
             sf::Vector2f startAcceleration = sf::Vector2f(0.0f, 0.0f), float rotationSpeed = 0.0f, float scaleRate = 0.0f,
             float alphaDecayRate = 0.0f);

    void updateParticle(float dt);

    void drawParticle(sf::RenderWindow &window) const;

    bool isParticleDead() const;

    sf::Vector2f getPosition() const;

    sf::Vector2f getVelocity() const;

    void setPosition(sf::Vector2f newPosition);

    void setVelocity(sf::Vector2f startVelocity);
};
