#pragma once
#include "GameState.hpp"
#include <stack>
#include <memory>

class Game {
    sf::RenderWindow m_window;
    sf::Font m_font;
    sf::Texture m_smokeTexture;
    std::stack<std::unique_ptr<GameState> > m_statesStack;

public:
    explicit Game(sf::Font &font, sf::Texture &texture);

    ~Game();

    void run();

    void pushState(std::unique_ptr<GameState> state);

    void popState();

    void changeState(std::unique_ptr<GameState> state);

    sf::RenderWindow &getWindow();

    sf::Font &getFont();

    sf::Texture &getSmokeTexture();
};
