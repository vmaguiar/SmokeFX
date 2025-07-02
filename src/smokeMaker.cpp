#include "smokeMaker.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

#include "configConsts.hpp"


SmokeMaker::SmokeMaker(sf::Vector2f position, sf::Color mainColor, sf::Color outlineColor, int maxParticles,
                       float particleLifeTime, sf::Color particleColor,
                       float particleSize, sf::Vector2f initialDirection, float initialSpeed,
                       float particlePerSecond): m_ONsmokeMakerShapePointer(sf::PrimitiveType::Triangles, 3),
                                                 m_ONsmokeMakerShapePointerOutline(sf::PrimitiveType::Triangles, 3),
                                                 m_position(position),
                                                 m_mainColor(mainColor),
                                                 m_outlineColor(outlineColor),
                                                 m_maxParticles(maxParticles),
                                                 m_particleLifetime(particleLifeTime),
                                                 m_particleColor(particleColor),
                                                 m_particleSize(particleSize),
                                                 m_currentLaunchDirection(initialDirection),
                                                 m_particleInitialSpeed(initialSpeed),
                                                 m_particlesPerSecond(particlePerSecond),
                                                 m_spawnAccumulator(0.0f) {
    // Off Shape
    m_OFFsmokeMakerShape.setRadius(config::EMITTER_RADIUS);
    m_OFFsmokeMakerShape.setFillColor(sf::Color(255, 128, 0)); // Orange
    m_OFFsmokeMakerShape.setOutlineThickness(5.0f);
    m_OFFsmokeMakerShape.setOutlineColor(sf::Color(192, 192, 192)); // Gray
    m_OFFsmokeMakerShape.setOrigin({config::EMITTER_RADIUS, config::EMITTER_RADIUS});
    m_OFFsmokeMakerShape.setPosition(m_position);

    // On Shape ->
    m_ONsmokeMakerShape = m_OFFsmokeMakerShape;


    m_enabledFeatures[SimulationFeature::ConstantSpeed] = true; // always active
    m_enabledFeatures[SimulationFeature::SmoothStop] = false;
    m_enabledFeatures[SimulationFeature::DecreasingAlpha] = false;
    m_enabledFeatures[SimulationFeature::IncreasingSize] = false;
    m_enabledFeatures[SimulationFeature::Rotation] = false;
    m_enabledFeatures[SimulationFeature::Texture] = false;
    m_enabledFeatures[SimulationFeature::SteamEffect] = false;

    updateActiveSmokeMakerVisuals();
}

void SmokeMaker::spawnNewParticles(float dt) {
    m_spawnAccumulator += dt;
    float particlesToSpawn = m_spawnAccumulator * m_particlesPerSecond;
    // at least 1 particle to spawn
    if (particlesToSpawn >= 1.0f) {
        int count = static_cast<int>(particlesToSpawn);
        for (int i = 0; i < count; i++) {
            if (m_particles.size() < m_maxParticles) {
                // Logic for particles features config

                // Basics Particle proprieties
                sf::Vector2f particleAcceleration = {0.0f, 0.0f};
                float particleInitialRotationSpeed = 0.0f;
                float particleScaleRate = 0.0f;
                float particleInitialMaxAlphaDecaySpeed = 0.0f;
                // float particleInitialMaxAlpha = 0.0f;
                float velKParam = 0.0f;
                float alphaKParam = 0.0f;
                float sizeKParam = 0.0f;

                // here there may be a small random direction variation


                // 1. Smooth Stop Configs
                if (m_enabledFeatures[SimulationFeature::SmoothStop]) {
                    if (m_particleLifetime > 0) {
                        velKParam = m_velKConst;
                    }
                }
                else {
                    velKParam = 0.0f;
                }


                // 2. Decreasing Alpha Configs
                if (m_enabledFeatures[SimulationFeature::DecreasingAlpha]) {
                    if (m_particleLifetime > 0) {
                        particleInitialMaxAlphaDecaySpeed = (static_cast<float>(m_particleColor.a) / m_particleLifetime);
                        // particleInitialMaxAlpha = static_cast<float>(m_particleColor.a);
                        alphaKParam = m_alphaKConst;
                    }
                }
                else {
                    alphaKParam = 0.0f;
                }


                // 3. Increasing Size Configs
                if (m_enabledFeatures[SimulationFeature::IncreasingSize]) {
                    if (m_particleLifetime > 0) {
                        particleScaleRate = m_maxParticleSize / m_particleLifetime;
                        sizeKParam = m_sizeKConst;
                    }
                }
                else {
                    sizeKParam = 0.0f;
                }


                // 4. Rotation Configs
                if (m_enabledFeatures[SimulationFeature::Rotation]) {
                    if (m_particleLifetime > 0) {
                        particleInitialRotationSpeed = m_rotationPerLifeTime / m_particleLifetime;
                    }
                }


                // 5. Texture Configs
                // W.I.P


                // 6. Steam Effect Configs
                if (m_enabledFeatures[SimulationFeature::SteamEffect]) {
                    particleAcceleration = m_steamEffectAccelerationVect;
                    if (m_particleLifetime > 0) {
                    }
                }
                else {
                    // particleAcceleration = {0.0f, 0.0f};
                }


                m_particles.emplace_back(m_position, m_currentLaunchDirection, m_particleInitialSpeed, velKParam,
                                         m_particleColor, particleInitialMaxAlphaDecaySpeed, alphaKParam,
                                         m_particleSize, m_maxParticleSize, sizeKParam,
                                         m_particleLifetime,
                                         particleAcceleration, particleInitialRotationSpeed, m_rotKConst, particleScaleRate);
            }
        }
        m_spawnAccumulator = m_spawnAccumulator - (static_cast<float>(count) / m_particlesPerSecond);
    }
}

void SmokeMaker::updateParticles(float dt) {
    for (Particle &particle: m_particles) {
        particle.update(dt);
    }
}

void SmokeMaker::removeDeadParticles() {
    m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(),
                                     [](const Particle &p) { return p.isDead(); }),
                      m_particles.end());
}

sf::Vector2f SmokeMaker::calculateAimDirection(sf::Vector2f target, sf::Vector2f source) {
    sf::Vector2f direction = target - source;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length == 0) {
        length = 1.0f;
    }
    return direction / length;
}

void SmokeMaker::updateActiveSmokeMakerVisuals() {
    float pointerLength = config::EMITTER_RADIUS * 2.15f;
    float pointerWidth = config::EMITTER_RADIUS * 1.0f;
    float outlineThickness = m_ONsmokeMakerShape.getOutlineThickness();

    sf::Vector2f perpendicularDirection = sf::Vector2f(-m_currentLaunchDirection.y, m_currentLaunchDirection.x);


    // tip of triangle
    sf::Vector2f p0_pointer = m_position + (m_currentLaunchDirection * pointerLength);
    sf::Vector2f p0_outline = m_position + (m_currentLaunchDirection * (pointerLength + outlineThickness));

    // base of the triangle
    // top point
    sf::Vector2f p1_pointer = m_position - (perpendicularDirection * pointerWidth);
    sf::Vector2f p1_outline = m_position - (perpendicularDirection * (pointerWidth - outlineThickness));
    // low point
    sf::Vector2f p2_pointer = m_position + (perpendicularDirection * pointerWidth);
    sf::Vector2f p2_outline = m_position + (perpendicularDirection * (pointerWidth + outlineThickness));

    // Pointer (orange triangle)
    m_ONsmokeMakerShapePointer[0].position = p0_pointer;
    m_ONsmokeMakerShapePointer[0].color = m_mainColor;
    m_ONsmokeMakerShapePointer[1].position = p1_pointer;
    m_ONsmokeMakerShapePointer[1].color = m_mainColor;
    m_ONsmokeMakerShapePointer[2].position = p2_pointer;
    m_ONsmokeMakerShapePointer[2].color = m_mainColor;

    // Outline (gray triangle)
    m_ONsmokeMakerShapePointerOutline[0].position = p0_outline;
    m_ONsmokeMakerShapePointerOutline[0].color = m_outlineColor;
    m_ONsmokeMakerShapePointerOutline[1].position = p1_outline;
    m_ONsmokeMakerShapePointerOutline[1].color = m_outlineColor;
    m_ONsmokeMakerShapePointerOutline[2].position = p2_outline;
    m_ONsmokeMakerShapePointerOutline[2].color = m_outlineColor;
}


void SmokeMaker::update(float dt) {
    if (m_isActive) {
        spawnNewParticles(dt);
        updateActiveSmokeMakerVisuals();
    }
    updateParticles(dt);
    removeDeadParticles();
}

void SmokeMaker::draw(sf::RenderWindow &window) {
    for (Particle &particle: m_particles) {
        particle.draw(window);
    }

    if (m_isActive) {
        window.draw(m_ONsmokeMakerShape);
        window.draw(m_ONsmokeMakerShapePointerOutline);
        window.draw(m_ONsmokeMakerShapePointer);
    }
    else {
        window.draw(m_OFFsmokeMakerShape);
    }
}


void SmokeMaker::setPosition(sf::Vector2f newPosition) {
    m_position = newPosition;
    m_OFFsmokeMakerShape.setPosition(m_position);
    m_ONsmokeMakerShape.setPosition(m_position);

    updateActiveSmokeMakerVisuals();
}

void SmokeMaker::setAimTarget(sf::Vector2f targetPos) {
    m_currentLaunchDirection = calculateAimDirection(targetPos, m_position);
    updateActiveSmokeMakerVisuals();
}

void SmokeMaker::setIsActive(bool active) {
    m_isActive = active;
}

void SmokeMaker::setEnabledFeatures(const std::map<SimulationFeature, bool> &features) {
    m_enabledFeatures = features;
    // always active
    m_enabledFeatures[SimulationFeature::ConstantSpeed] = true;
}

sf::Vector2f SmokeMaker::getPosition() const {
    return m_position;
}


void SmokeMaker::adjustParticleVelKConst(float delta) {
    m_velKConst = m_velKConst + delta;
    if (m_velKConst < 0.0f) {
        m_velKConst = 0.0f;
    }
    std::cout << "Constant k for smooth stop: " << m_velKConst << std::endl;
}


void SmokeMaker::adjustParticleAlphaKConst(float delta) {
    m_alphaKConst = m_alphaKConst + delta;
    std::cout << "Constant k for Decreasing Alpha: " << m_alphaKConst << std::endl;
}


void SmokeMaker::adjustParticleSizeKConst(float delta) {
    m_sizeKConst = m_sizeKConst + delta;
    std::cout << "Constant k for Increasing Size: " << std::endl;
}


void SmokeMaker::adjustParticleMaxSize(float delta) {
    m_maxParticleSize = m_maxParticleSize + delta;
    std::cout << "Max particle Size for Increasing Size: " << std::endl;
}


void SmokeMaker::adjustParticleRotKConst(float delta) {
    m_rotKConst = m_rotKConst + delta;
    std::cout << "Const k for Rotation speed decay: " << m_rotationPerLifeTime << std::endl;
}


void SmokeMaker::adjustRotationSpeedMultiplier(float delta) {
    m_rotationPerLifeTime = m_rotationPerLifeTime + (delta * 360.0f);
    std::cout << "lap multiplier: " << m_rotationPerLifeTime << std::endl;
}


void SmokeMaker::adjustSteamEffectAccelerationVec(float delta) {
    m_steamEffectAccelerationVect = {m_steamEffectAccelerationVect.x, m_steamEffectAccelerationVect.y + delta};
    std::cout << "Steam Effect Vector: {" << m_steamEffectAccelerationVect.x << ", " << m_steamEffectAccelerationVect.y << "}" <<
            std::endl;
}


float SmokeMaker::getParticleVelKConst() const {
    return m_velKConst;
}


float SmokeMaker::getParticleAlphaKConst() const {
    return m_alphaKConst;
}


float SmokeMaker::getSizeKConst() const {
    return m_sizeKConst;
}


float SmokeMaker::getMaxParticleSize() const {
    return m_maxParticleSize;
}


float SmokeMaker::getParticleRotKConst() const {
    return m_rotKConst;
}


float SmokeMaker::getRotationSpeedMultiplier() const {
    return m_rotationPerLifeTime;
}

sf::Vector2f SmokeMaker::getSteamEffectAccelerationVect() const {
    return m_steamEffectAccelerationVect;
}
