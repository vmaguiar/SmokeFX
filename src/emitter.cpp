#include "emitter.hpp"
#include "configConsts.hpp"
#include <cmath>

Emitter::Emitter(const sf::Vector2f emitterStartPosition): m_EmitterPosition(emitterStartPosition) {
    // Circle config (idle)
    m_idleEmitterShape.setRadius(config::EMITTER_RADIUS);
    m_idleEmitterShape.setFillColor(sf::Color(255, 128, 0)); // Orange
    m_idleEmitterShape.setOutlineThickness(5.0f);
    m_idleEmitterShape.setOutlineColor(sf::Color(192, 192, 192)); // Gray
    m_idleEmitterShape.setOrigin({config::EMITTER_RADIUS, config::EMITTER_RADIUS});
    m_idleEmitterShape.setPosition(m_EmitterPosition);

    m_activeEmitterBaseShape = m_idleEmitterShape;
}

void Emitter::spawnNewParticlesHandler(std::vector<Particle> &particles, float dt) {
    if (m_isCasting) {
        m_spawnTimer = m_spawnTimer + dt;
        if (m_spawnTimer > config::PARTICLE_SPAWN_TIME) {
            // std::cout << spawnTimer << std::endl;
            sf::Vector2f particleSpeed = m_currentLaunchDirection * config::PARTICLE_INIT_SPEED;
            particles.emplace_back(m_EmitterPosition, particleSpeed);
            // std::cout << "num de particulas = " << smokeParticles.size() << std::endl;
            m_spawnTimer = m_spawnTimer - config::PARTICLE_SPAWN_TIME;
        }
    }
}


void Emitter::updateEmitter(sf::Vector2f mousePosition) {
    m_currentLaunchDirection = mousePosition - m_EmitterPosition;
    float lengthToMouse = std::sqrt(
        m_currentLaunchDirection.x * m_currentLaunchDirection.x + m_currentLaunchDirection.y * m_currentLaunchDirection.y);
    if (lengthToMouse == 0.0f) {
        lengthToMouse = 1.0f;
    }
    m_currentLaunchDirection = m_currentLaunchDirection / lengthToMouse; // Normalizado
    sf::Vector2f perpendicularDirection = {-m_currentLaunchDirection.y, m_currentLaunchDirection.x};
    float pointerTipLength = 20.0f;

    m_activeEmitterPointerShape.clear();
    m_activeEmitterPointerOutline.clear();
    m_activeEmitterPointerShape.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_activeEmitterPointerOutline.setPrimitiveType(sf::PrimitiveType::Triangles);

    sf::Color emitterColor = sf::Color(255, 128, 0);
    sf::Color emitterOutlineColor = sf::Color(192, 192, 192);


    // Orange triangle
    sf::Vector2f connectionPoint = m_EmitterPosition + m_currentLaunchDirection * config::EMITTER_RADIUS;

    sf::Vector2f basePoint1 = m_EmitterPosition + (perpendicularDirection * config::EMITTER_RADIUS);
    sf::Vector2f basePoint2 = m_EmitterPosition - (perpendicularDirection * config::EMITTER_RADIUS);

    sf::Vector2f tipPoint = m_EmitterPosition + m_currentLaunchDirection * (config::EMITTER_RADIUS + pointerTipLength);

    m_activeEmitterPointerShape.append({basePoint1, emitterColor});
    m_activeEmitterPointerShape.append({tipPoint, emitterColor});
    m_activeEmitterPointerShape.append({connectionPoint, emitterColor});

    m_activeEmitterPointerShape.append({basePoint2, emitterColor});
    m_activeEmitterPointerShape.append({tipPoint, emitterColor});
    m_activeEmitterPointerShape.append({connectionPoint, emitterColor});


    // Gray triangle
    float outlineThickness = m_activeEmitterBaseShape.getOutlineThickness();

    // float outlineHalfWidth = config::EMITTER_RADIUS + (outlineThickness * 0.5f);
    float outlineTipLength = pointerTipLength + outlineThickness;

    sf::Vector2f pointerOutlineConnection = m_EmitterPosition + m_currentLaunchDirection * config::EMITTER_RADIUS;;

    sf::Vector2f baseOutlinePoint1 = pointerOutlineConnection + (
                                         perpendicularDirection * (config::EMITTER_RADIUS + outlineThickness * 0.5f));
    sf::Vector2f baseOutilinePoint2 = pointerOutlineConnection - (
                                          perpendicularDirection * (config::EMITTER_RADIUS + outlineThickness * 0.5f));

    sf::Vector2f outlineTipPoint = m_EmitterPosition + m_currentLaunchDirection * (config::EMITTER_RADIUS + outlineTipLength);

    // 1
    m_activeEmitterPointerOutline.append({baseOutlinePoint1, emitterOutlineColor});
    m_activeEmitterPointerOutline.append({outlineTipPoint, emitterOutlineColor});
    m_activeEmitterPointerOutline.append({pointerOutlineConnection, emitterOutlineColor});

    // 2
    m_activeEmitterPointerOutline.append({baseOutilinePoint2, emitterOutlineColor});
    m_activeEmitterPointerOutline.append({outlineTipPoint, emitterOutlineColor});
    m_activeEmitterPointerOutline.append({pointerOutlineConnection, emitterOutlineColor});

    // sf::Vector2f outlineConnectionTop = m_EmitterPosition + perpendicularDirection * (config::EMITTER_RADIUS + outlineThickness);
    // sf::Vector2f outlineConnectionBottom = m_EmitterPosition - perpendicularDirection * (
    //                                            config::EMITTER_RADIUS + outlineThickness);
    //
    // sf::Vector2f outlineTip = m_EmitterPosition + m_currentLaunchDirection * (config::EMITTER_RADIUS + outlineTipLength);
    //
    // m_activeEmitterPointerOutline.append({outlineConnectionTop, emitterOutlineColor});
    // m_activeEmitterPointerOutline.append({outlineTip, emitterOutlineColor});
    // m_activeEmitterPointerOutline.append({outlineConnectionBottom, emitterOutlineColor});
    // m_activeEmitterPointerOutline.append({outlineConnectionTop, emitterOutlineColor});
}


void Emitter::draw(sf::RenderWindow &window) const {
    if (m_isCasting) {
        window.draw(m_idleEmitterShape);
        // window.draw(m_activeEmitterBaseShape);
        // window.draw(m_activeEmitterPointerShape);
        // window.draw(m_activeEmitterPointerOutline);
    }
    else {
        // window.draw(m_idleEmitterShape);
        window.draw(m_activeEmitterPointerOutline);
        window.draw(m_activeEmitterBaseShape);
        window.draw(m_activeEmitterPointerShape);
    }
}

void Emitter::setCasting(bool isCasting) {
    m_isCasting = isCasting;
}

sf::Vector2f Emitter::getPosition() const {
    return m_EmitterPosition;
}
