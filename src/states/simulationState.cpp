#include "simulationState.hpp"
#include <iostream>
#include "../configConsts.hpp"
#include "../game.hpp"

SimulationState::SimulationState(Game &game, sf::RenderWindow &window): m_game(game),
                                                                        m_window(window),
                                                                        m_smokeMaker(
                                                                            config::EMITTER_START_POSITION,
                                                                            config::EMITTER_MAIN_COLOR,
                                                                            config::EMITTER_OUTILINE_COLOR,
                                                                            config::MAX_PARTICLES, config::PARTICLE_LIFETIME,
                                                                            sf::Color::White, config::PARTICLE_SIZE,
                                                                            sf::Vector2f({1.0f, 0.0f}),
                                                                            config::PARTICLE_INIT_SPEED,
                                                                            config::PARTICLE_SPAWN_RATE),
                                                                        m_font(m_game.getFont()),
                                                                        m_featureStatusText(m_font, "texto inicial", 18),
                                                                        m_isSmokeMakerActive(false) {
    m_activeFeatures[SimulationFeature::ConstantSpeed] = true;
    m_activeFeatures[SimulationFeature::SmoothStop] = false;
    m_activeFeatures[SimulationFeature::DecreasingAlpha] = false;
    m_activeFeatures[SimulationFeature::IncreasingSize] = false;
    m_activeFeatures[SimulationFeature::Rotation] = false;
    m_activeFeatures[SimulationFeature::Texture] = false;
    m_activeFeatures[SimulationFeature::SteamEffect] = false;

    applyFeaturesToSmokeMaker();
    updateFeatureStatusText();
}

SimulationState::~SimulationState() = default;

void SimulationState::handleEvent(const sf::Event &event) {
    if (event.is<sf::Event::MouseMoved>()) {
        const auto *mouseMoved = event.getIf<sf::Event::MouseMoved>();
        m_smokeMaker.setAimTarget(static_cast<sf::Vector2f>(mouseMoved->position));
    }
    if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            m_isSmokeMakerActive = true;
            m_smokeMaker.setIsActive(true);
        }
    }
    else if (const auto *mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            m_isSmokeMakerActive = false;
            m_smokeMaker.setIsActive(false);
        }
    }
    else if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        float step = 0.1f;
        if (keyPressed->scancode == sf::Keyboard::Scancode::LShift || keyPressed->scancode == sf::Keyboard::Scancode::RShift) {
            step = 0.01f;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::LControl || keyPressed->scancode ==
                 sf::Keyboard::Scancode::RControl) {
            step = 1.0f;
        }
        switch (keyPressed->scancode) {
            //Smooth Stop
            case sf::Keyboard::Scancode::Num1:
                m_activeFeatures[SimulationFeature::SmoothStop] = !m_activeFeatures[SimulationFeature::SmoothStop];
                std::cout << "Smooth Stop: " << (m_activeFeatures[SimulationFeature::SmoothStop] ? "ON" : "OFF") << std::endl;
                break;

            // Decreasing Alpha
            case sf::Keyboard::Scancode::Num2:
                m_activeFeatures[SimulationFeature::DecreasingAlpha] = !m_activeFeatures[SimulationFeature::DecreasingAlpha];
                std::cout << "Decreasing Alpha: " << (m_activeFeatures[SimulationFeature::DecreasingAlpha] ? "ON" : "OFF") <<
                        std::endl;
                break;

            // Increasing Size
            case sf::Keyboard::Scancode::Num3:
                m_activeFeatures[SimulationFeature::IncreasingSize] = !m_activeFeatures[SimulationFeature::IncreasingSize];
                std::cout << "Increasing Size: " << (m_activeFeatures[SimulationFeature::IncreasingSize] ? "ON" : "OFF") <<
                        std::endl;
                break;

            // Rotation
            case sf::Keyboard::Scancode::Num4:
                m_activeFeatures[SimulationFeature::Rotation] = !m_activeFeatures[SimulationFeature::Rotation];
                std::cout << "Rotation: " << (m_activeFeatures[SimulationFeature::Rotation] ? "ON" : "OFF") << std::endl;
                break;

            // Texture
            // case sf::Keyboard::Scancode::Num5:
            //     m_activeFeatures[SimulationFeature::Texture] = !m_activeFeatures[SimulationFeature::Texture];
            //     std::cout << "Texture: " << (m_activeFeatures[SimulationFeature::Texture] ? "ON" : "OFF") << std::endl;
            //     break;

            // Steam Effect
            case sf::Keyboard::Scancode::Num6:
                m_activeFeatures[SimulationFeature::SteamEffect] = !m_activeFeatures[SimulationFeature::SteamEffect];
                std::cout << "Steam Effect: " << (m_activeFeatures[SimulationFeature::SteamEffect] ? "ON" : "OFF") << std::endl;
                break;

            // Velocity Decay Adjustment (Q/A)
            case sf::Keyboard::Scancode::Q:
                m_smokeMaker.adjustParticleVelDecayConst(step);
                break;
            case sf::Keyboard::Scancode::A:
                m_smokeMaker.adjustParticleVelDecayConst(-step);
                break;

            // Decreasing Alpha Multiplier (W/S)
            // Size Growth Multiplier (E/D)

            // Rotation Multiplier (R/F)
            case sf::Keyboard::Scancode::R:
                m_smokeMaker.adjustRotationSpeedMultiplier(step);
                break;
            case sf::Keyboard::Scancode::F:
                m_smokeMaker.adjustRotationSpeedMultiplier(-step);
                break;

            // Steam Effect Multiplier (T/G)

            case sf::Keyboard::Scancode::Escape:
                m_game.popState();
                break;
            default:
                break;
        }

        applyFeaturesToSmokeMaker();
        updateFeatureStatusText();
    }
}

void SimulationState::update(float dt) {
    m_smokeMaker.update(dt);
}

void SimulationState::draw(sf::RenderWindow &window) {
    window.clear();
    m_smokeMaker.draw(window);
    window.draw(m_featureStatusText);
}

void SimulationState::updateFeatureStatusText() {
    std::string statusString = "Funcionalidades Ativas:\n";
    statusString += "1 - Smooth Stop: " + std::string(m_activeFeatures[SimulationFeature::SmoothStop] ? "ON" : "OFF") + "\n";
    statusString += "2 - Decreasing Alpha: " + std::string(m_activeFeatures[SimulationFeature::DecreasingAlpha] ? "ON" : "OFF") +
            "\n";
    statusString += "3 - Increasing Size: " + std::string(m_activeFeatures[SimulationFeature::IncreasingSize] ? "ON" : "OFF") +
            "\n";
    statusString += "4 - Rotation: " + std::string(m_activeFeatures[SimulationFeature::Rotation] ? "ON" : "OFF") + "\n";
    // statusString += "5 - Texture: " + std::string(m_activeFeatures[SimulationFeature::Texture] ? "ON" : "OFF") + "\n";
    statusString += "6 - Steam Effect: " + std::string(m_activeFeatures[SimulationFeature::SteamEffect] ? "ON" : "OFF") + "\n";

    statusString += "\n-- Ajustes (Q/A, W/S, E/D, R/F) --\n";
    statusString += "Smooth Stop Const: " + std::to_string(m_smokeMaker.getAdjustParticleVelDecayConst()) + "\n";
    statusString += "Alpha Decay M: \n";
    statusString += "Size Growth M: \n";
    statusString += "Rotation Speed M: " + std::to_string(m_smokeMaker.getAdjustRotationSpeedMultiplier()) + "\n";
    statusString += "Steam Accel M: \n";

    statusString += "\nMouse Esquerdo: ON/OFF emissor\n";
    statusString += "ESC: Sair";

    m_featureStatusText.setString(statusString);
}

void SimulationState::applyFeaturesToSmokeMaker() {
    m_smokeMaker.setEnabledFeatures(m_activeFeatures);
}
