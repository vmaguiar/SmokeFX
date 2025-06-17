#pragma once
#include "SFML/Graphics.hpp"

class GameState;
class Game;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void handleEvent(const sf::Event &event);

    virtual void update(float dt);

    virtual void draw(sf::RenderWindow &window);
};
