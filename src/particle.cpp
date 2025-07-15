#include "particle.hpp"
#include <cmath>
#include <iostream>

Particle::Particle(sf::Vector2f startPosition, sf::Vector2f startVelocityDirection, float velocityMagnitude, float velDecayRate,
                   sf::Color startColor, float alphaDecayRate, float alphaKConst,
                   float startSize, float maxSize, float sizeKConst, float lifeTime, sf::Texture *smokeTexturePtr,
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
                                     m_texturePtr(smokeTexturePtr),
                                     m_spawnTime(std::chrono::high_resolution_clock::now()),
                                     m_lifetime(lifeTime),
                                     m_totalElapsedTime(0.0f) {
    m_shape.setSize({m_initialSize, m_initialSize});
    m_shape.setFillColor(m_initialColor);
    m_shape.setOrigin({m_initialSize * 0.5f, m_initialSize * 0.5f}); // origin in the middle of the square
    m_shape.setPosition(startPosition);

    m_currentVelocity = m_velocityDirection * m_initialMaxVelocityMagnitude;

    if (m_texturePtr) {
        m_smokeSpritePtr = std::make_unique<sf::Sprite>(*m_texturePtr);
        m_smokeSpritePtr->setOrigin({
            static_cast<float>(m_smokeSpritePtr->getTexture().getSize().x) / 2.0f,
            static_cast<float>(m_smokeSpritePtr->getTexture().getSize().y) / 2.0f
        });
        m_smokeSpritePtr->setScale({
            m_initialSize / static_cast<float>(m_smokeSpritePtr->getTexture().getSize().x),
            m_initialSize / static_cast<float>(m_smokeSpritePtr->getTexture().getSize().y)
        });
        m_smokeSpritePtr->setColor(m_initialColor);
        m_smokeSpritePtr->setPosition(startPosition);
    }
}

void Particle::update(float dt, const std::vector<sf::FloatRect> &walls) {
    m_totalElapsedTime = m_totalElapsedTime + dt;

    // if particle is already dead don't update
    if (isDead()) {
        return;
    }

    sf::FloatRect particleBounds;
    if (m_smokeSpritePtr) {
        // Obter os limites globais do sprite (se estiver usando textura)
        particleBounds = m_smokeSpritePtr->getGlobalBounds();
    }
    else {
        // Obter os limites globais da forma (se não estiver usando textura)
        particleBounds = m_shape.getGlobalBounds();
    }


    // 1. Apply acceleration for smooth stop and steam effect
    // Vel(t) = VelInitialMax * e^(-k*t)
    float currentSpeedMagnitude = m_initialMaxVelocityMagnitude * std::exp(-m_velDecayRate * m_totalElapsedTime);
    m_currentVelocity = m_velocityDirection * currentSpeedMagnitude;

    // If we have other accelerations (like gravity, steam effect) that are *additive*
    // to the base exponential movement, apply them *after* calculating currentVelocity.

    // 6. Apply Steam Effect
    m_currentVelocity = m_currentVelocity + (m_acceleration * dt);

    m_shape.move(dt * m_currentVelocity);


    // 2. Apply decreasing alpha
    if (m_currentAlpha != 0.0f) {
        m_currentAlpha = m_initialMaxAlpha * std::exp(-m_alphaKConst * m_totalElapsedTime);

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
        m_currentSize = std::max(0.0f, m_currentSize);

        m_shape.setSize({m_currentSize, m_currentSize});
        m_shape.setOrigin({m_currentSize * 0.5f, m_currentSize * 0.5f});
    }


    // 4. Apply rotation
    if (m_initialMaxRotationSpeed != 0.0f) {
        // Omega(t) = OmegaInitialMax * e^(-k*t)
        m_currentRotationSpeed = m_initialMaxRotationSpeed * std::exp(-m_rotDecayRate * m_totalElapsedTime);
        m_currentRotation = m_currentRotationSpeed;

        if (m_currentRotation >= 360.0f) {
            m_currentRotation -= 360.0f;
        }
        else if (m_currentRotation < 0.0f) {
            m_currentRotation += 360.0f;
        }
        m_shape.setRotation(sf::degrees(m_currentRotation));
    }


    // 5. Apply Texture
    if (m_smokeSpritePtr) {
        // acceleration
        m_smokeSpritePtr->move(dt * m_currentVelocity);

        // alpha
        sf::Color currentColor = m_shape.getFillColor();
        currentColor.a = static_cast<uint8_t>(m_currentAlpha);
        m_smokeSpritePtr->setColor(currentColor);

        // size
        m_smokeSpritePtr->setScale({
            m_currentSize / static_cast<float>(m_smokeSpritePtr->getTexture().getSize().x),
            m_currentSize / static_cast<float>(m_smokeSpritePtr->getTexture().getSize().y)
        });
        m_smokeSpritePtr->setOrigin({
            static_cast<float>(m_smokeSpritePtr->getTexture().getSize().x) / 2.0f,
            static_cast<float>(m_smokeSpritePtr->getTexture().getSize().y) / 2.0f
        });

        // rotation
        m_smokeSpritePtr->setRotation(sf::degrees(m_currentRotation));
    }

    for (const auto &wall: walls) {
        if (particleBounds.findIntersection(wall)) {
            // collision detected
            // 1 overlap calculation
            float overlapX = std::min(particleBounds.position.x + particleBounds.size.x, wall.position.x + wall.size.x) -
                             std::max(particleBounds.position.x, wall.position.x);
            float overlapY = std::min(particleBounds.position.y + particleBounds.size.y, wall.position.y + wall.size.y) -
                             std::max(particleBounds.position.y, wall.position.y);

            // 2 intersection axis
            if (overlapX < overlapY) {
                // horizontal collision
                m_currentVelocity.x = m_currentVelocity.x * -1.0f;
                if (particleBounds.position.x < wall.position.x) {
                    // left collision
                    m_shape.setPosition({wall.position.x - particleBounds.size.x / 2.0f, m_shape.getPosition().y});
                }
                else {
                    // right collision
                    m_shape.setPosition({wall.position.x + wall.size.x + particleBounds.size.x / 2.0f, m_shape.getPosition().y});
                }
            }
            else {
                // vertical collision
                m_currentVelocity.y = m_currentVelocity.y * -1.0f;
                if (particleBounds.position.y < wall.position.y) {
                    // upper collision
                    m_shape.setPosition({m_shape.getPosition().x, wall.position.y - particleBounds.size.y / 2.0f});
                }
                else {
                    // bottom collision
                    m_shape.setPosition({m_shape.getPosition().x, wall.position.y + wall.size.y + particleBounds.size.y / 2.0f});
                }
            }

            if (m_smokeSpritePtr) {
                m_smokeSpritePtr->setPosition({m_shape.getPosition().x, m_shape.getPosition().y});
            }
        }
    }
}


void Particle::draw(sf::RenderWindow &window) const {
    if (isDead()) {
        return;
    }
    if (m_smokeSpritePtr) {
        if (m_smokeSpritePtr->getColor().a > 0) {
            window.draw(*m_smokeSpritePtr);
        }
    }
    else {
        if (m_shape.getFillColor().a > 0) {
            window.draw(m_shape);
        }
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
