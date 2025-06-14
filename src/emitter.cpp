#include "emitter.hpp"
#include "configConsts.hpp"

Emitter::Emitter(const sf::Vector2f emitterStartPosition): m_EmitterPosition(emitterStartPosition) {
    // Circle config (idle)
    m_idleEmitterShape.setRadius(config::EMITTER_RADIUS);
    m_idleEmitterShape.setFillColor(sf::Color(255, 128, 0)); // Orange
    m_idleEmitterShape.setOutlineThickness(3.0f);
    m_idleEmitterShape.setOutlineColor(sf::Color(192, 192, 192)); // Gray
    m_idleEmitterShape.setOrigin({config::EMITTER_RADIUS, config::EMITTER_RADIUS});
    m_idleEmitterShape.setPosition(m_EmitterPosition);

    // Arrow config (active)
    m_activeEmitterShape.setPointCount(5);
    m_activeEmitterShape.setPoint(0, sf::Vector2f(0.0f, 0.0f)); // Center
    m_activeEmitterShape.setPoint(1, sf::Vector2f(config::EMITTER_RADIUS * 2, -config::EMITTER_RADIUS)); // Up left point
    m_activeEmitterShape.setPoint(2, sf::Vector2f(config::EMITTER_RADIUS * 3, 0.0f)); // Right point
    m_activeEmitterShape.setPoint(3, sf::Vector2f(config::EMITTER_RADIUS * 2, config::EMITTER_RADIUS)); // Down left point
    m_activeEmitterShape.setPoint(4, sf::Vector2f(0.0f, 0.0f)); // Center again
    m_activeEmitterShape.setFillColor(sf::Color(255, 128, 0)); // Orange
    m_activeEmitterShape.setOrigin({0.0f, 0.0f});
    m_activeEmitterShape.setPosition(m_EmitterPosition);
}

void Emitter::draw(sf::RenderWindow &window) const {
    if (m_isCasting) {
        window.draw(m_activeEmitterShape);
    }
    else {
        window.draw(m_idleEmitterShape);
    }
}

void Emitter::setCasting(bool isCasting) {
    m_isCasting = isCasting;
}

sf::Vector2f Emitter::getPosition() const {
    return m_EmitterPosition;
}
