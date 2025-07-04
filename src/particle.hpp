#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>

class Particle {
    sf::RectangleShape m_shape;
    sf::Vector2f m_velocityDirection;
    float m_initialMaxVelocityMagnitude;
    sf::Vector2f m_currentVelocity;
    float m_velDecayRate = 0.0f;
    sf::Vector2f m_acceleration;


    // rotation
    float m_currentRotation; // degrees
    float m_initialMaxRotationSpeed; // degrees per sec
    float m_currentRotationSpeed = 0.0f; // degrees per sec
    float m_rotDecayRate;

    // size
    float m_initialSize;
    float m_currentSize;
    float m_maxSize;
    float m_scaleRate; // pixels per seconds ou %/sec
    float m_sizeKConst = 0.0f;

    // color & alpha
    sf::Color m_initialColor;
    float m_initialMaxAlpha = 255.0f;
    float m_currentAlpha; // (0-255) empty - full
    // float m_currentAlphaDecay = 0.0f;
    float m_alphaKConst;
    float m_alphaDecayRate; // alpha per seconds

    sf::Texture *m_texturePtr;
    // sf::Sprite *m_smokeSpritePtr;
    std::unique_ptr<sf::Sprite> m_smokeSpritePtr;

    std::chrono::high_resolution_clock::time_point m_spawnTime;
    float m_lifetime; // total in seconds
    float m_totalElapsedTime;

public:
    Particle(sf::Vector2f startPosition, sf::Vector2f startVelocityDirection, float velocityMagnitude, float velDecayRate,
             sf::Color startColor, float alphaDecayRate, float alphaKConst, float startSize, float maxSize, float sizeKConst,
             float lifeTime, sf::Texture *smokeTexturePtr,
             sf::Vector2f startAcceleration = sf::Vector2f(0.0f, 0.0f), float initialMaxRotationSpeed = 0.0f,
             float rotDecayRate = 0.0f, float scaleRate = 0.0f);

    void update(float dt);

    void draw(sf::RenderWindow &window) const;

    bool isDead() const;

    sf::Vector2f getPosition() const;

    sf::Vector2f getVelocity() const;

    void setPosition(sf::Vector2f newPosition);

    void setVelocity(sf::Vector2f startVelocity);
};
