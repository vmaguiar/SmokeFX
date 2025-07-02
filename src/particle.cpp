#include "particle.hpp"
#include <cmath>
#include <iostream>
/*to do:
 * - rotation can't be perfect turns every time (maybe with randomness it will solve)
 * - need some randomness into rotation
 * - maybe some randomness for the increasing size too
 * - randomness to the direction of the particles will be necessary :S
 * - put some texture on
*/
Particle::Particle(sf::Vector2f startPosition, sf::Vector2f startVelocityDirection, float velocityMagnitude, float velDecayRate,
                   sf::Color startColor, float alphaDecayRate, float alphaKConst,
                   float startSize, float maxSize, float sizeKConst, float lifeTime,
                   sf::Vector2f startAcceleration, float initialMaxRotationSpeed, float rotDecayRate,
                   float scaleRate): m_velocityDirection(startVelocityDirection),
                                     m_initialMaxVelocityMagnitude(velocityMagnitude),
                                     m_velDecayRate(velDecayRate),
                                     m_acceleration(startAcceleration),
                                     m_currentRotation(0.0f),
                                     m_initialMaxRotationSpeed(initialMaxRotationSpeed),
                                     m_rotDecayRate(rotDecayRate),
                                     m_initialSize(startSize),
                                     m_currentSize(startSize),
                                     m_maxSize(maxSize),
                                     m_scaleRate(scaleRate),
                                     m_sizeKConst(sizeKConst),
                                     m_initialColor(startColor),
                                     m_currentAlpha(static_cast<float>(startColor.a)),
                                     m_alphaKConst(alphaKConst),
                                     m_alphaDecayRate(alphaDecayRate),
                                     m_spawnTime(std::chrono::high_resolution_clock::now()),
                                     m_lifetime(lifeTime),
                                     m_totalElapsedTime(0.0f) {
    m_shape.setSize({m_initialSize, m_initialSize});
    m_shape.setFillColor(m_initialColor);
    m_shape.setOrigin({m_initialSize * 0.5f, m_initialSize * 0.5f}); // origin in the middle of the square
    m_shape.setPosition(startPosition);

    m_currentVelocity = m_velocityDirection * m_initialMaxVelocityMagnitude;
}

void Particle::update(float dt) {
    m_totalElapsedTime = m_totalElapsedTime + dt;

    // if particle is already dead don't update
    if (isDead()) {
        return;
    }

    // 1. Apply acceleration for smooth stop and steam effect
    // Vel(t) = VelInitialMax * e^(-k*t)
    float currentSpeedMagnitude = m_initialMaxVelocityMagnitude * std::exp(-m_velDecayRate * m_totalElapsedTime);
    m_currentVelocity = m_velocityDirection * currentSpeedMagnitude;

    // If you have other accelerations (like gravity, steam effect) that are *additive*
    // to the base exponential movement, apply them *after* calculating currentVelocity.
    // For example: currentVelocity += m_acceleration * dt; // if m_acceleration is gravity

    // 6. Apply Steam Effect
    m_currentVelocity = m_currentVelocity + (m_acceleration * dt);

    m_shape.move(dt * m_currentVelocity);


    // 2. Apply decreasing alpha
    if (m_currentAlpha != 0.0f) {
        m_currentAlpha = m_initialMaxAlpha * std::exp(-m_alphaKConst * m_totalElapsedTime);
        // m_currentAlpha = m_currentAlpha - (m_currentAlphaDecay * dt);
        // m_currentAlpha -= m_alphaDecayRate * dt;

        m_currentAlpha = std::max(0.0f, m_currentAlpha);
        m_currentAlpha = std::min(255.0f, m_currentAlpha);

        sf::Color currentColor = m_shape.getFillColor();
        currentColor.a = static_cast<uint8_t>(m_currentAlpha);
        m_shape.setFillColor(currentColor);
    }


    // 3. Apply increasing size
    if (m_scaleRate != 0.0f) {
        float currentSizeGrowth = m_maxSize * (1.0f - std::exp(-m_sizeKConst * m_totalElapsedTime));
        m_currentSize = currentSizeGrowth;
        // m_currentSize = m_currentSize + m_currentSize * (currentSizeGrowth * dt);
        // m_currentSize += m_scaleRate * dt;
        m_currentSize = std::max(0.0f, m_currentSize);

        m_shape.setSize({m_currentSize, m_currentSize});
        m_shape.setOrigin({m_currentSize * 0.5f, m_currentSize * 0.5f});
    }


    // 4. Apply rotation
    if (m_initialMaxRotationSpeed != 0.0f) {
        // Omega(t) = OmegaInitialMax * e^(-k*t)
        m_currentRotationSpeed = m_initialMaxRotationSpeed * std::exp(-m_rotDecayRate * m_totalElapsedTime);
        m_currentRotation = m_currentRotationSpeed;
        // m_currentRotation = m_currentRotation + (m_currentRotationSpeed * dt);

        if (m_currentRotation >= 360.0f) {
            m_currentRotation -= 360.0f;
        }
        else if (m_currentRotation < 0.0f) {
            m_currentRotation += 360.0f;
        }
        m_shape.setRotation(sf::degrees(m_currentRotation));
    }
}


void Particle::draw(sf::RenderWindow &window) const {
    if (m_shape.getFillColor().a > 0) {
        window.draw(m_shape);
    }
}


bool Particle::isDead() const {
    return m_totalElapsedTime >= m_lifetime || m_currentAlpha <= 0.0f;
}


sf::Vector2f Particle::getPosition() const {
    return m_shape.getPosition();
}

sf::Vector2f Particle::getVelocity() const {
    return m_currentVelocity;
}

void Particle::setPosition(sf::Vector2f newPosition) {
    m_shape.setPosition(newPosition);
}

void Particle::setVelocity(sf::Vector2f startVelocity) {
    m_velocityDirection = startVelocity;
}
