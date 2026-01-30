#include "stdafx.h"

#include "MenuState.hpp"

#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "TGUI/TGUI.hpp"


namespace mmt_gd
{
using namespace std;

void MenuState::init()
{
    PROFILE_FUNCTION();

    if (m_isInit)
    {
        return;
    }

    if (!m_font.loadFromFile("../assets/arial.ttf"))
    {
        sf::err() << "Could not load font\n";
        return;
    }
    m_text.setPosition(m_game->getWindow().getView().getCenter());
    m_text.setString("Press space to continue");
    m_text.setFillColor(sf::Color::White);
    m_text.setFont(m_font);
    m_text.setOrigin(m_text.getLocalBounds().width * 0.5F, m_text.getLocalBounds().height * 0.5F);

    m_view = m_game->getWindow().getView();

    m_isInit = true;


    // LoadGui
    m_game->getGui().loadWidgetsFromFile("../assets/demoTgui.txt");

    if (const auto btn = dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("StartGameBtn")))
    {
        btn->onClick([&manager = m_gameStateManager] { manager->setState("MainState"); });
    }
}

void MenuState::update(float delta)
{
    PROFILE_FUNCTION();

    m_game->getWindow().setView(m_view);

    if (InputManager::getInstance().isKeyPressed("Select"))
    {
        m_gameStateManager->setState("MainState");
    }
}

void MenuState::draw()
{
    PROFILE_FUNCTION();

    m_game->getWindow().draw(m_text);
}

void MenuState::exit()
{
    PROFILE_FUNCTION();

    m_game->getGui().removeAllWidgets();
    GameState::exit();
}
} // namespace mmt_gd
