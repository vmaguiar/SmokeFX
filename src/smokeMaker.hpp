#pragma once
#include <map>
#include <vector>

#include "configConsts.hpp"
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
    sf::Color m_mainColor = sf::Color(255, 128, 0);
    sf::Color m_outlineColor = sf::Color(192, 192, 192);
    int m_maxParticles;
    float m_particleLifetime;
    sf::Color m_particleColor;
    float m_particleSize;
    sf::Vector2f m_currentLaunchDirection;
    float m_particleInitialSpeed;
    float m_particlesPerSecond;
    float m_spawnAccumulator;
    sf::Texture *m_particleTexturePtr;

    // config var
    float m_velKConst = config::PARTICLE_VEL_DECAY_CONST;
    float m_alphaKConst = config::PARTICLE_VEL_DECAY_CONST;
    float m_sizeKConst = config::PARTICLE_VEL_DECAY_CONST;
    float m_maxParticleSize = 600.0f;
    float m_rotKConst = config::PARTICLE_VEL_DECAY_CONST;
    float m_rotationPerLifeTime = config::ROTATION_PER_LIFETIME;
    sf::Vector2f m_steamEffectAccelerationVect = {0.0f, -1500.0f};


    std::vector<Particle> m_particles;
    bool m_isActive = false;

    std::map<SimulationFeature, bool> m_enabledFeatures;


    void spawnNewParticles(float dt);

    void updateParticles(float dt);

    void removeDeadParticles();

    static sf::Vector2f calculateAimDirection(sf::Vector2f target, sf::Vector2f source);

    void updateActiveSmokeMakerVisuals();

public:
    SmokeMaker(sf::Vector2f position, sf::Color mainColor, sf::Color outlineColor, int maxParticles, float particleLifeTime,
               sf::Color particleColor, float particleSize,
               sf::Vector2f initialDirection, float initialSpeed, float particlePerSecond, sf::Texture *particleTexturePtr);

    // methods SimulationState will call
    void update(float dt);

    void draw(sf::RenderWindow &window);

    // external control methods
    void setPosition(sf::Vector2f newPosition);

    void setAimTarget(sf::Vector2f targetPos);

    void setIsActive(bool active);

    // methods to pass features flags
    void setEnabledFeatures(const std::map<SimulationFeature, bool> &features);

    // utils
    sf::Vector2f getPosition() const;

    void adjustParticleVelKConst(float delta);

    void adjustParticleAlphaKConst(float delta);

    void adjustParticleSizeKConst(float delta);

    void adjustParticleMaxSize(float delta);

    void adjustParticleRotKConst(float delta);

    void adjustRotationSpeedMultiplier(float delta);

    void adjustSteamEffectAccelerationVec(float delta);

    float getParticleVelKConst() const;

    float getParticleAlphaKConst() const;

    float getSizeKConst() const;

    float getMaxParticleSize() const;

    float getParticleRotKConst() const;

    float getRotationSpeedMultiplier() const;

    sf::Vector2f getSteamEffectAccelerationVect() const;
};
