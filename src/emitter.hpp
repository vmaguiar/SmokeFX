#pragma once
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "particle.hpp"

class Emitter {
    sf::CircleShape m_idleEmitterShape;
    sf::CircleShape m_activeEmitterBaseShape;
    sf::VertexArray m_activeEmitterPointerShape;
    sf::VertexArray m_activeEmitterPointerOutline;

    sf::Vector2f m_currentLaunchDirection;

    sf::Vector2f m_EmitterPosition;
    float m_spawnTimer = 0.0f;

public:
    bool m_isCasting = false;

    explicit Emitter(sf::Vector2f emitterStartPosition);

    void spawnNewParticlesHandler(std::vector<Particle> &particles, float dt);

    void updateEmitter(sf::Vector2f mousePosition);

    void draw(sf::RenderWindow &window) const;

    void setCasting(bool isCasting);

    sf::Vector2f getPosition() const;
};
