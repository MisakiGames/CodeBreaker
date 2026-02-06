#include "stdafx.h"

#include "Game.hpp"

#include "DebugDraw.hpp"
#include "EndState.hpp"
#include "InputManager.hpp"
#include "MainState.hpp"
#include "MenuState.hpp"

#include <sstream>

namespace mmt_gd
{
using namespace std;

void Game::run()
{
    Instrumentor::instance().beginSession("startup", "startup.json");

    if (!init())
    {
        return;
    }
    Instrumentor::instance().endSession();
    Instrumentor::instance().beginSession("runtime", "runtime.json");

    while (m_window.isOpen() && !InputManager::getInstance().isKeyDown("Exit"))
    {
        PROFILE_SCOPE("Frame");

        {
            PROFILE_SCOPE("Process Events");

            // process events in the input manager
            sf::Event event{};
            while (m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    shutdown();
                    m_window.close();
                    return;
                }
                m_inputManager->process(event);
                m_gui.handleEvent(event);
            }
        }
        update();
        draw();
    }

    shutdown();
}

bool Game::init()
{
    PROFILE_FUNCTION();

    m_inputManager = &InputManager::getInstance();
    m_inputManager->bind("Exit", sf::Keyboard::Escape, 0);

    m_debugDraw = &DebugDraw::getInstance();
    m_window.setFramerateLimit(120);
    //
    m_gameStateManager.registerState("MenuState", make_shared<MenuState>(&m_gameStateManager, this));
    m_gameStateManager.registerState("MainState", make_shared<MainState>(&m_gameStateManager, this));

    //
    m_window.create(sf::VideoMode(m_config.m_resolution.x, m_config.m_resolution.y),
                    m_config.m_windowName,
                    sf::Style::Fullscreen);
    m_gui.setTarget(m_window);
    m_gui.setFont("../assets/font.ttf");

    m_inputManager->setRenderWindow(&m_window);

    m_gameStateManager.setState("MenuState");

    return true;
}

void Game::update()
{
    PROFILE_FUNCTION();

    // starts the clock
    static sf::Clock clock;
    // restart takes time
    const auto deltaTime        = clock.restart();
    const auto deltaTimeSeconds = deltaTime.asSeconds();

    // must be first call
    m_inputManager->update();

    m_gameStateManager.update(deltaTimeSeconds);

    m_debugDraw->update(deltaTimeSeconds);

    std::ostringstream ss;
    m_fps.update();
    ss << m_config.m_windowName << " | FPS: " << m_fps.getFps();

    m_window.setTitle(ss.str());
}

void Game::draw()
{
    PROFILE_FUNCTION();

    m_window.clear();

    m_gameStateManager.draw();

    m_debugDraw->draw(m_window);

    m_gui.draw();

    m_window.display();
}

void Game::shutdown() const
{
    m_gameStateManager.shutdown();
    InputManager::getInstance().clear();
}
} // namespace mmt_gd