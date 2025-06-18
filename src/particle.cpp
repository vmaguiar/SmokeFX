#include "particle.hpp"

#include <iostream>

Particle::Particle(sf::Vector2f startPosition, sf::Vector2f startVelocity, sf::Color startColor, float startSize, float lifeTime,
                   sf::Vector2f startAcceleration, float rotationSpeed, float scaleRate,
                   float alphaDecayRate): m_particleVelocity(startVelocity),
                                          m_particleAcceleration(startAcceleration),
                                          m_particleCurrentRotation(0.0f),
                                          m_particleRotationSpeed(rotationSpeed),
                                          m_particleInitialSize(startSize),
                                          m_particleCurrentSize(startSize),
                                          m_particleScaleRate(scaleRate),
                                          m_particleInitialColor(startColor),
                                          m_particleCurrentAlpha(static_cast<float>(startColor.a)),
                                          m_particleDecayRate(alphaDecayRate),
                                          m_spawnTime(std::chrono::high_resolution_clock::now()),
                                          m_particleLifetime(lifeTime) {
    m_particleShape.setSize({m_particleInitialSize, m_particleInitialSize});
    m_particleShape.setFillColor(m_particleInitialColor);
    m_particleShape.setOrigin({m_particleInitialSize * 0.5f, m_particleInitialSize * 0.5f}); // origin in the middle of the square
    m_particleShape.setPosition(startPosition);
}

void Particle::updateParticle(float dt) {
    auto now = std::chrono::high_resolution_clock::now();
    float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float> >(now - m_spawnTime).count();

    // if particle is already dead dont update
    if (elapsedTime >= m_particleLifetime && m_particleCurrentAlpha <= 0.0f) {
        return;
    }

    // 1. Apply acceleration for smooth stop and steam effect
    m_particleVelocity += m_particleAcceleration * dt;
    m_particleShape.move(dt * m_particleVelocity);

    // 2. Apply rotation
    if (m_particleRotationSpeed != 0.0f) {
        m_particleCurrentRotation += m_particleRotationSpeed * dt;

        if (m_particleCurrentRotation >= 360.0f) {
            m_particleCurrentRotation -= 360.0f;
        }
        else if (m_particleCurrentRotation < 0.0f) {
            m_particleCurrentRotation += 360.0f;
        }
        m_particleShape.setRotation(sf::degrees(m_particleRotationSpeed));
    }

    // 3. Apply increasing size
    if (m_particleScaleRate != 0.0f) {
        m_particleCurrentSize += m_particleScaleRate * dt;
        m_particleCurrentSize = std::max(0.0f, m_particleCurrentSize);

        m_particleShape.setSize({m_particleCurrentSize, m_particleCurrentSize});
        m_particleShape.setOrigin({m_particleCurrentSize * 0.5f, m_particleCurrentSize * 0.5f});
    }

    // 4. Apply decreasing alpha
    if (m_particleCurrentAlpha != 0.0f) {
        m_particleCurrentAlpha -= m_particleDecayRate * dt;

        m_particleCurrentAlpha = std::max(0.0f, m_particleCurrentAlpha);
        m_particleCurrentAlpha = std::min(255.0f, m_particleCurrentAlpha);

        sf::Color currentColor = m_particleShape.getFillColor();
        currentColor.a = static_cast<uint8_t>(m_particleCurrentAlpha);
        m_particleShape.setFillColor(currentColor);
    }
}

void Particle::drawParticle(sf::RenderWindow &window) const {
    if (m_particleShape.getFillColor().a > 0) {
        window.draw(m_particleShape);
    }
}

bool Particle::isParticleDead() const {
    auto now = std::chrono::high_resolution_clock::now();
    float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float> >(now - m_spawnTime).count();
    return elapsedTime >= m_particleLifetime || m_particleCurrentAlpha <= 0.0f;
}


sf::Vector2f Particle::getPosition() const {
    return m_particleShape.getPosition();
}

sf::Vector2f Particle::getVelocity() const {
    return m_particleVelocity;
}

void Particle::setPosition(sf::Vector2f newPosition) {
    m_particleShape.setPosition(newPosition);
}

void Particle::setVelocity(sf::Vector2f startVelocity) {
    m_particleVelocity = startVelocity;
}
