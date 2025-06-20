#pragma once

#include "../gameState.hpp"
#include "../smokeMaker.hpp"

class Game;

class SimulationState : public GameState {
    Game &m_game;
    sf::RenderWindow &m_window;

    SmokeMaker m_smokeMaker;

    std::map<SimulationFeature, bool> m_activeFeatures;

    sf::Font m_font;
    sf::Text m_featureStatusText;

    bool m_isSmokeMakerActive;

    void updateFeatureStatusText();

    void applyFeaturesToSmokeMaker();

public:
    SimulationState(Game &game, sf::RenderWindow &window);

    ~SimulationState() override;

    void handleEvent(const sf::Event &event) override;

    void update(float dt) override;

    void draw(sf::RenderWindow &window) override;
};
