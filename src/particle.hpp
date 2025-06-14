#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

class Particle {
    sf::RectangleShape m_particleShape;
    // sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    // Particle configs
    sf::Vector2f m_particleShapeSize = {10.0f, 10.0f};
    sf::Color m_particleShapeColor = sf::Color::White;

public:
    Particle(sf::Vector2f startPosition, sf::Vector2f startVelocity);

    void updateParticle(float dt);

    void draw(sf::RenderWindow &window) const;

    sf::Vector2f getPosition() const;

    sf::Vector2f getVelocity() const;

    void setPosition(sf::Vector2f newPosition);

    void setVelocity(sf::Vector2f startVelocity);
};
