#include <iostream>
#include "Game.hpp"
#include "states/MenuState.hpp"

int main() {
    sf::Font gameFont;
    if (!gameFont.openFromFile("assets/fonts/Roboto-Italic.ttf")) {
        std::cerr << "Erro ao carregar fonte para o MenuState!" << std::endl;
    }
    Game game(gameFont);

    game.pushState(std::make_unique<MenuState>(game));

    game.run();

    return 0;
}
