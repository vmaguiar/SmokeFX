#include "game.hpp"
#include "configConsts.hpp"
// Incluir os estados posteriormente

Game::Game() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    m_window.create(sf::VideoMode(config::WINDOW_SIZE), "SmokeFX", sf::State::Windowed, settings);
    m_window.setFramerateLimit(config::MAX_FRAMERATE);
}

Game::~Game() {
    while (!m_statesStack.empty()) {
        m_statesStack.pop();
    }
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        const float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            }
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    if (m_statesStack.size() > 1) {
                        popState();
                    }
                    else {
                        m_window.close();
                    }
                    continue;
                }
            }

            if (!m_statesStack.empty()) {
                m_statesStack.top()->handleEvent(*event);
            }
        }

        // Updating
        if (!m_statesStack.empty()) {
            m_statesStack.top()->update(deltaTime);
        }
        else {
            // Close or back to default state
            m_window.close();
        }

        // Drawing
        m_window.clear();
        if (!m_statesStack.empty()) {
            m_statesStack.top()->draw(m_window);
        }

        m_window.display();
    }
}

void Game::pushState(std::unique_ptr<GameState> state) {
    if (!m_statesStack.empty()) {
        // Se o estado atual precisar de uma pausa, implemente aqui
        // m_states.top()->pause();
    }
    m_statesStack.push(std::move(state));
}

void Game::popState() {
    if (!m_statesStack.empty()) {
        m_statesStack.pop();
    }
}

void Game::changeState(std::unique_ptr<GameState> state) {
    popState();
    m_statesStack.push(std::move(state));
}

sf::RenderWindow &Game::getWindow() {
    return m_window;
}
