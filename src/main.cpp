#include "Game.hpp"
#include "states/MenuState.hpp"

int main() {
    Game game;

    game.pushState(std::make_unique<MenuState>(game));

    game.run();

    return 0;
}
