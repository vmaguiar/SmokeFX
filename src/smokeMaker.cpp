#include "smokeMaker.hpp"

#include <algorithm>
#include <cmath>
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
                                                 m_initialSpeed(initialSpeed),
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
    // 1 second
    if (particlesToSpawn >= 1.0f) {
        int count = static_cast<int>(particlesToSpawn);
        for (int i = 0; i < count; i++) {
            if (m_particles.size() < m_maxParticles) {
                // Logic for particles features config

                // Basics Particle proprieties
                sf::Vector2f particleVelocity = m_currentLaunchDirection * m_initialSpeed;
                sf::Vector2f particleAcceleration = sf::Vector2f(0.0f, 0.0f);
                float particleRotationSpeed = 0.0f;
                float particleScaleRate = 0.0f;
                float particleAlphaDecayRate = 0.0f;

                // Desvio aleatório para a direção (sempre presente para efeito de spray)
                // Nao sei ainda se faço

                // 1. Smooth Stop Configs
                if (m_enabledFeatures[SimulationFeature::SmoothStop]) {
                    if (m_particleLifetime > 0) {
                        particleAcceleration += -particleVelocity / m_particleLifetime;
                    }
                }

                // 2. Decreasing Alpha Configs
                if (m_enabledFeatures[SimulationFeature::DecreasingAlpha]) {
                }

                // 3. Increasing Size Configs
                if (m_enabledFeatures[SimulationFeature::IncreasingSize]) {
                }

                // 4. Rotation Configs
                if (m_enabledFeatures[SimulationFeature::Rotation]) {
                }

                // 5. Steam Effect Configs
                if (m_enabledFeatures[SimulationFeature::SteamEffect]) {
                }

                // 6. Texture Configs
                // W.I.P

                m_particles.emplace_back(m_position, particleVelocity, m_particleColor, m_particleSize, m_particleLifetime,
                                         particleAcceleration, particleRotationSpeed, particleScaleRate, particleAlphaDecayRate);
            }
        }
        m_spawnAccumulator -= static_cast<float>(count);
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

    // m_ONsmokeMakerShapePointer.clear();
    // m_ONsmokeMakerShapePointerOutline.clear();

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

