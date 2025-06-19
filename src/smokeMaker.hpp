#pragma once
#include <map>
#include <vector>
#include "particle.hpp"
#include "SFML/Graphics/CircleShape.hpp"

enum class SimulationFeature {
    ConstantSpeed, // always active
    SmoothStop,
    DecreasingAlpha,
    IncreasingSize,
    Rotation,
    Texture,
    SteamEffect
};


class SmokeMaker {
    // Off
    sf::CircleShape m_OFFsmokeMakerShape;
    // On
    sf::CircleShape m_ONsmokeMakerShape;
    sf::VertexArray m_ONsmokeMakerShapePointer;
    sf::VertexArray m_ONsmokeMakerShapePointerOutline;

    sf::Vector2f m_position;
    int m_maxParticles;
    float m_particleLifetime;
    sf::Color m_particleColor;
    float m_particleSize;
    sf::Vector2f m_currentLaunchDirection;
    float m_initialSpeed;
    float m_particlesPerSecond;
    float m_spawnAccumulator;

    std::vector<Particle> m_particles;
    bool m_isActive = false;

    std::map<SimulationFeature, bool> m_enabledFeatures;


    void spawnNewParticles(float dt);

    void updateParticles(float dt);

    void removeDeadParticles();

    static sf::Vector2f calculateAimDirection(sf::Vector2f target, sf::Vector2f source);

public:
    SmokeMaker(sf::Vector2f position, int maxParticles, float particleLifeTime, sf::Color particleColor, float particleSize,
               sf::Vector2f inicialDirection, float initialSpeed, float particlePerSecond);

    // methods SimulationState will call
    void update(float dt);

    void draw(sf::RenderWindow &window);

    // external control methods
    void setPosition(sf::Vector2f newPosition);

    void setAimTarget(sf::Vector2f targetPos);

    void setIsActive(bool active);

    // methods to pass feature flags
    void setEnabledFeatures(const std::map<SimulationFeature, bool> &features);

    // utils
    sf::Vector2f getPosition() const;
};
