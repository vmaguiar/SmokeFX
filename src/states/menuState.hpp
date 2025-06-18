#pragma once

#include "../gameState.hpp"
#include "../game.hpp"
#include "constantSpeedState.hpp"
#include <SFML/Graphics.hpp>

class Game;

class MenuState : public GameState {
    Game &m_game;
    sf::Font m_font;
    std::optional<sf::Text> m_titleText;
    std::vector<sf::Text> m_menuItems;

    int m_selectedItemIndex;

    void setupText();

    void selectNextItem();

    void selectPreviousItem();

    void activateSelectedItem();

public:
    explicit MenuState(Game &game);

    ~MenuState() override = default;

    // Inherited methods
    void handleEvent(const sf::Event &event) override;

    void update(float dt) override;

    void draw(sf::RenderWindow &window) override;
};
