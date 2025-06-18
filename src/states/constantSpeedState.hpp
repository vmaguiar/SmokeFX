#pragma once
#include "../gameState.hpp"
#include "../game.hpp"
#include "../emitter.hpp"

class Game;

class ConstantSpeedState : public GameState {
    Game &m_game;
    Emitter m_emitter;

public:
    explicit ConstantSpeedState(Game &game);

    ~ConstantSpeedState() override = default;

    // Inherited methods
    void handleEvent(const sf::Event &event) override;

    void update(float dt);

    void draw(sf::RenderWindow &window);
};
