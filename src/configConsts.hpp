#pragma once
#include <SFML/Graphics.hpp>

namespace config {
    // Window Config
    constexpr sf::Vector2u WINDOW_SIZE = {1920u, 1080u};
    constexpr sf::Vector2f WINDOW_SIZE_F = static_cast<sf::Vector2f>(WINDOW_SIZE);
    constexpr uint8_t MAX_FRAMERATE = 60;
    constexpr float CONFIG_DELTA_TIME = 1.0f / static_cast<float>(MAX_FRAMERATE);

    // Emitter Config
    constexpr float EMITTER_RADIUS = 20.0f;
    constexpr sf::Vector2f EMITTER_START_POSITION = sf::Vector2f(50.0f, WINDOW_SIZE_F.y * 0.5f);
}
