#include "particle.hpp"

#include <iostream>

Particle::Particle(sf::Vector2f startPosition, sf::Vector2f startVelocity, sf::Color startColor, float startSize, float lifeTime,
                   sf::Vector2f startAcceleration, float rotationSpeed, float scaleRate,
                   float alphaDecayRate): m_velocity(startVelocity),
                                          m_acceleration(startAcceleration),
                                          m_currentRotation(0.0f),
                                          m_rotationSpeed(rotationSpeed),
                                          m_initialSize(startSize),
                                          m_currentSize(startSize),
                                          m_scaleRate(scaleRate),
                                          m_initialColor(startColor),
                                          m_currentAlpha(static_cast<float>(startColor.a)),
                                          m_decayRate(alphaDecayRate),
                                          m_spawnTime(std::chrono::high_resolution_clock::now()),
                                          m_lifetime(lifeTime) {
    m_shape.setSize({m_initialSize, m_initialSize});
    m_shape.setFillColor(m_initialColor);
    m_shape.setOrigin({m_initialSize * 0.5f, m_initialSize * 0.5f}); // origin in the middle of the square
    m_shape.setPosition(startPosition);
}

void Particle::update(float dt) {
    auto now = std::chrono::high_resolution_clock::now();
    float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float> >(now - m_spawnTime).count();

    // if particle is already dead dont update
    if (elapsedTime >= m_lifetime && m_currentAlpha <= 0.0f) {
        return;
    }

    // 1. Apply acceleration for smooth stop and steam effect
    m_velocity += m_acceleration * dt;
    m_shape.move(dt * m_velocity);

    // 2. Apply rotation
    if (m_rotationSpeed != 0.0f) {
        m_currentRotation += m_rotationSpeed * dt;

        if (m_currentRotation >= 360.0f) {
            m_currentRotation -= 360.0f;
        }
        else if (m_currentRotation < 0.0f) {
            m_currentRotation += 360.0f;
        }
        m_shape.setRotation(sf::degrees(m_rotationSpeed));
    }

    // 3. Apply increasing size
    if (m_scaleRate != 0.0f) {
        m_currentSize += m_scaleRate * dt;
        m_currentSize = std::max(0.0f, m_currentSize);

        m_shape.setSize({m_currentSize, m_currentSize});
        m_shape.setOrigin({m_currentSize * 0.5f, m_currentSize * 0.5f});
    }

    // 4. Apply decreasing alpha
    if (m_currentAlpha != 0.0f) {
        m_currentAlpha -= m_decayRate * dt;

        m_currentAlpha = std::max(0.0f, m_currentAlpha);
        m_currentAlpha = std::min(255.0f, m_currentAlpha);

        sf::Color currentColor = m_shape.getFillColor();
        currentColor.a = static_cast<uint8_t>(m_currentAlpha);
        m_shape.setFillColor(currentColor);
    }
}


void Particle::draw(sf::RenderWindow &window) const {
    if (m_shape.getFillColor().a > 0) {
        window.draw(m_shape);
    }
}


bool Particle::isDead() const {
    auto now = std::chrono::high_resolution_clock::now();
    float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float> >(now - m_spawnTime).count();
    return elapsedTime >= m_lifetime || m_currentAlpha <= 0.0f;
}


sf::Vector2f Particle::getPosition() const {
    return m_shape.getPosition();
}

sf::Vector2f Particle::getVelocity() const {
    return m_velocity;
}

void Particle::setPosition(sf::Vector2f newPosition) {
    m_shape.setPosition(newPosition);
}

void Particle::setVelocity(sf::Vector2f startVelocity) {
    m_velocity = startVelocity;
}
