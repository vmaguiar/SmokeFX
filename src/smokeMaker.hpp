#pragma once
#include "SFML/Graphics/CircleShape.hpp"

class SmokeMaker {
    // Off
    sf::CircleShape m_OFFsmokeMakerShape;
    // On
    sf::CircleShape m_ONsmokeMakerShape;
    sf::VertexArray m_ONsmokeMakerShapePointer;
    sf::VertexArray m_ONsmokeMakerShapePointerOutline;

    sf::Vector2f m_currentLaunchDirection;

public:
    SmokeMaker();
};
