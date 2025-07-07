#pragma once
#include <SFML/Graphics.hpp>

namespace config {
    // Window Config
    constexpr sf::Vector2u WINDOW_SIZE = {1920u, 1080u};
    constexpr sf::Vector2f WINDOW_SIZE_F = static_cast<sf::Vector2f>(WINDOW_SIZE);
    constexpr uint8_t MAX_FRAMERATE = 60;
    constexpr float CONFIG_DELTA_TIME = 1.0f / static_cast<float>(MAX_FRAMERATE);

    // World Config
    // emitter
    constexpr float EMITTER_RADIUS = 10.0f;
    constexpr sf::Vector2f EMITTER_START_POSITION = sf::Vector2f(50.0f, WINDOW_SIZE_F.y * 0.5f);
    constexpr sf::Color EMITTER_MAIN_COLOR = sf::Color(255, 128, 0);
    constexpr sf::Color EMITTER_OUTLINE_COLOR = sf::Color(192, 192, 192);
    constexpr int MAX_PARTICLES = 2000;
    // particle
    constexpr float PARTICLE_LIFETIME = 20.0f;
    constexpr float PARTICLE_INIT_SPEED = 300.0f;
    constexpr float PARTICLE_SPAWN_RATE = 20.0f; // particle per seconds
    constexpr float PARTICLE_SPAWN_TIME = 1.0f / PARTICLE_SPAWN_RATE; // 0.05...
    constexpr float PARTICLE_SIZE = 40.0f;
    constexpr float PARTICLE_MAX_SIZE = 650.0f;
    //
    constexpr float PARTICLE_VEL_K_CONST = 0.2f;
    constexpr float PARTICLE_ALPHA_K_CONST = 0.3f;
    constexpr float PARTICLE_SIZE_K_CONST = 0.2f;
    constexpr float PARTICLE_ROT_K_CONST = 0.3f;
    constexpr float TOTAL_ROTATIONS = 1080.0f; //3 * 360
    constexpr sf::Vector2f STEAM_EFFECT_VECTOR = {0.0f, -2250.0f};
}
