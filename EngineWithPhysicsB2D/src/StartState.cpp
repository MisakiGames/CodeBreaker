#include "stdafx.hpp"

#include "StartState.hpp"

#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include <TGUI/TGUI.hpp>

namespace mmt_gd
{
void StartState::init()
{
    PROFILE_FUNCTION();

    try
    {
        m_game->getGui().loadWidgetsFromFile("../assets/startTgui.txt");
    } catch (const tgui::Exception& e)
    {
        sf::err() << "TGUI Load Error: " << e.what() << std::endl;
        return;
    }

    InputManager::getInstance().bind("Skip", sf::Keyboard::Space, 0);

    m_accumulatedTime = 0.0f;
}

void StartState::update(float delta)
{
    PROFILE_FUNCTION();
    m_accumulatedTime += delta;

    if (m_accumulatedTime >= m_displayDuration)
    {
        m_gameStateManager->setState("MenuState");
    }

    if (InputManager::getInstance().isKeyPressed("Skip"))
    {
        m_gameStateManager->setState("MenuState");
    }
}

void StartState::draw()
{
    PROFILE_FUNCTION();
    return;
}

void StartState::exit()
{
    PROFILE_FUNCTION();
    InputManager::getInstance().unbind("Skip", 0);
    m_game->getGui().removeAllWidgets();
    GameState::exit();
}
} // namespace mmt_gd