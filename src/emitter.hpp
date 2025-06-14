#pragma once
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Emitter {
    sf::CircleShape m_idleEmitterShape;
    sf::ConvexShape m_activeEmitterShape;
    sf::Vector2f m_EmitterPosition;

public:
    bool m_isCasting = false;

    explicit Emitter(sf::Vector2f emitterStartPosition);

    void setCasting(bool isCasting);

    void draw(sf::RenderWindow &window) const;

    sf::Vector2f getPosition() const;
};
