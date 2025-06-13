#pragma once
#include <SFML/Graphics.hpp>

namespace config {
    // Window Config
    constexpr sf::Vector2u windowSize = {1920u, 1080u};
    constexpr sf::Vector2f windowSize_f = static_cast<sf::Vector2f>(windowSize);
    constexpr uint8_t maxFramerate = 60;
    constexpr float configDeltaTime = 1.0f / static_cast<float>(maxFramerate);
}
