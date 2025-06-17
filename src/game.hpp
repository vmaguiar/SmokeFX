#pragma once
#include "GameState.hpp"
#include <stack>
#include <memory>

class Game {
    sf::RenderWindow m_window;
    std::stack<std::unique_ptr<GameState> > m_statesStack;

public:
    Game();

    ~Game();

    void run();

    void pushState(std::unique_ptr<GameState> state);

    void popState();

    void changeState(std::unique_ptr<GameState> state);

    sf::RenderWindow &getWindow();
};
