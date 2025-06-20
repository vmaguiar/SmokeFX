#include "menuState.hpp"
#include <iostream>
#include "../configConsts.hpp"

MenuState::MenuState(Game &game): m_game(game), m_selectedItemIndex(0) {
    if (!m_font.openFromFile("assets/fonts/Roboto-Italic.ttf")) {
        std::cerr << "Erro ao carregar fonte para o MenuState!" << std::endl;
    }

    m_titleText.emplace(m_font, "SmokeFX Simulator", 80);
    m_titleText->setFillColor(sf::Color::White);
    m_titleText->setOrigin({m_titleText->getLocalBounds().size.x / 2, m_titleText->getLocalBounds().size.y / 2});
    m_titleText->setPosition({config::WINDOW_SIZE_F.x / 2.0f, (config::WINDOW_SIZE_F.y / 2.0f) - 150});

    // Menu items config
    m_menuItems.emplace_back(m_font, "Simulation: Constant Speed", 50); // index = 0
    m_menuItems.emplace_back(m_font, "Exit", 50); // index = 1

    setupText();
}

void MenuState::setupText() {
    float totalHeight = 0;
    for (const sf::Text &item: m_menuItems) {
        totalHeight = totalHeight + item.getLocalBounds().size.y + 10;
    }
    float startY = (config::WINDOW_SIZE.y / 2.0f) - (totalHeight / 2.0f) + 50;

    for (size_t i = 0; i < m_menuItems.size(); i++) {
        m_menuItems[i].setFillColor(sf::Color::White);
        m_menuItems[i].setOrigin({m_menuItems[i].getLocalBounds().size.x / 2, m_menuItems[i].getLocalBounds().size.y / 2});
        m_menuItems[i].setPosition({config::WINDOW_SIZE_F.x / 2.0f, startY + (static_cast<float>(i) * 60)});
    }

    if (!m_menuItems.empty()) {
        m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Yellow);
    }
}

void MenuState::selectNextItem() {
    if (!m_menuItems.empty()) {
        m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::White);
        m_selectedItemIndex = (m_selectedItemIndex + 1) % static_cast<int>(m_menuItems.size());
        m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Yellow);
    }
}

void MenuState::selectPreviousItem() {
    if (!m_menuItems.empty()) {
        m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::White);
        m_selectedItemIndex = (m_selectedItemIndex - 1 + static_cast<int>(m_menuItems.size())) % static_cast<int>(m_menuItems.
                                  size());
        m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Yellow);
    }
}

void MenuState::activateSelectedItem() {
    if (m_menuItems.empty()) {
        return;
    }
    if (m_selectedItemIndex == 0) {
        // item Constant Speed Simulator
        m_game.pushState(std::make_unique<SimulationState>(m_game));
    }
    else if (m_selectedItemIndex == 1) {
        // item Exit
        m_game.getWindow().close();
    }
}

void MenuState::handleEvent(const sf::Event &event) {
    if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
            selectPreviousItem();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
            selectNextItem();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            activateSelectedItem();
        }
    }

    else if (event.is<sf::Event::MouseMoved>()) {
        const auto *mouseMoved = event.getIf<sf::Event::MouseMoved>();
        for (size_t i = 0; i < m_menuItems.size(); i++) {
            if (m_menuItems[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mouseMoved->position))) {
                if (m_selectedItemIndex != static_cast<int>(i)) {
                    if (!m_menuItems.empty()) {
                        m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::White);
                    }
                    m_selectedItemIndex = static_cast<int>(i);
                    m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Yellow);
                }
            }
        }
    }

    else if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            for (size_t i = 0; i < m_menuItems.size(); i++) {
                if (m_menuItems[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePressed->position))) {
                    m_selectedItemIndex = static_cast<int>(i);
                    activateSelectedItem();
                    break;
                }
            }
        }
    }
}

void MenuState::update(float dt) {
    (void) dt;
}

void MenuState::draw(sf::RenderWindow &window) {
    if (m_titleText.has_value()) {
        window.draw(*m_titleText);
    }

    for (const auto &item: m_menuItems) {
        window.draw(item);
    }
}
