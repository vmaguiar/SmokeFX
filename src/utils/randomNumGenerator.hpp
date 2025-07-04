#pragma once
#include <random>
#include <chrono>

class RandomNumberGenerator {
    static std::mt19937 m_engine;

public:
    RandomNumberGenerator() = delete;

    static void init() {
        m_engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }

    static int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(m_engine);
    }

    static float getFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(m_engine);
    }

    static bool getBool() {
        std::bernoulli_distribution dist(0.5);
        return dist(m_engine);
    }

    static int getDirection() {
        return (getInt(0, 1) == 0) ? -1 : 1;
    }
};
