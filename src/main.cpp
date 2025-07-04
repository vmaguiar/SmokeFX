#include <iostream>
#include "Game.hpp"
#include "states/MenuState.hpp"

int main() {
    sf::Font gameFont;
    if (!gameFont.openFromFile("assets/fonts/Roboto-Italic.ttf")) {
        std::cerr << "Error to load the Game font!" << std::endl;
    }
    sf::Texture smokeTexture;
    if (!smokeTexture.loadFromFile("assets/smoke.png")) {
        std::cerr << "Error to load Smoke.png texture!" << std::endl;
    }

    Game game(gameFont, smokeTexture);

    game.pushState(std::make_unique<MenuState>(game));

    game.run();

    return 0;
}
