#include "constantSpeedState.hpp"
#include "../configConsts.hpp"

ConstantSpeedState::ConstantSpeedState(Game &game): m_game(game), m_emitter(config::EMITTER_START_POSITION) {
}

void ConstantSpeedState::handleEvent(const sf::Event &event) {
    if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            m_game.popState();
        }
    }
}

void ConstantSpeedState::update(float dt) {
    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_game.getWindow()));
    m_emitter.updateEmitter(mousePosition);
}

void ConstantSpeedState::draw(sf::RenderWindow &window) {
    m_emitter.draw(window);
}
