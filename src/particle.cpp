#include "particle.hpp"

#include <iostream>

Particle::Particle(sf::Vector2f startPosition, sf::Vector2f startVelocity) {
    m_particleShape.setSize(m_particleShapeSize);
    m_particleShape.setOrigin(m_particleShape.getSize() * 0.5f);
    m_particleShape.setFillColor(m_particleShapeColor);
    m_particleShape.setPosition(startPosition);
    // m_position = startPosition;
    m_velocity = startVelocity;
}

void Particle::updateParticle(float dt) {
    m_particleShape.move(dt * m_velocity);
}

void Particle::draw(sf::RenderWindow &window) const {
    window.draw(m_particleShape);
}

sf::Vector2f Particle::getPosition() const {
    return m_particleShape.getPosition();
}

sf::Vector2f Particle::getVelocity() const {
    return m_velocity;
}

void Particle::setPosition(sf::Vector2f newPosition) {
    m_particleShape.setPosition(newPosition);
}

void Particle::setVelocity(sf::Vector2f startVelocity) {
    m_velocity = startVelocity;
}
