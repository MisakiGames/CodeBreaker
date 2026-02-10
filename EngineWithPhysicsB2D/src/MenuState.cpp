//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "MenuState.hpp"

#include "EventBus.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "GameObjectEvents.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "MusicComponent.h"
#include "TGUI/TGUI.hpp"


namespace mmt_gd
{
using namespace std;

void MenuState::init()
{
    PROFILE_FUNCTION();
    m_gameObjectManager.init();
    if (m_backgroundTexture.loadFromFile("../assets/menu_background.png"))
    {
        m_backgroundSprite.setTexture(m_backgroundTexture);

        sf::Vector2u textureSize = m_backgroundTexture.getSize();
        sf::Vector2u windowSize  = m_game->getWindow().getSize();

        m_backgroundSprite.setScale(static_cast<float>(windowSize.x) / textureSize.x,
                                    static_cast<float>(windowSize.y) / textureSize.y);
    }

    m_view = m_game->getWindow().getDefaultView();
    m_game->getWindow().setView(m_view);

    InputManager::getInstance().bind("Select", 0u, 0);
    InputManager::getInstance().bind("debugdraw", sf::Keyboard::F1, 0);


    try
    {
        m_game->getGui().loadWidgetsFromFile("../assets/menuTgui.txt");
    } catch (const tgui::Exception& e)
    {
        sf::err() << "TGUI Load Error: " << e.what() << std::endl;
        return;
    }

    if (const auto btn = dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("StartGameBtn")))
    {
        btn->onClick([&manager = m_gameStateManager] { manager->setState("MainState"); });
    }

    auto backgroundMusic = GameObject::create("BackgroundMusic");
    auto musicComponent  = backgroundMusic->addComponent<MusicComponent>(*backgroundMusic);
    musicComponent->addMusic("background", "../assets/musics/battlebgm.ogg", 100.0f);

    if (!backgroundMusic->init())
    {
        sf::err() << "Could not initialize music\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(backgroundMusic));

    musicComponent->playMusic("background", true);
}

void MenuState::update(float delta)
{
    PROFILE_FUNCTION();

    m_game->getWindow().setView(m_view);

    if (InputManager::getInstance().isKeyPressed("Select"))
    {
        m_gameStateManager->setState("MainState");
    }

    static float timer = 0;
    timer += delta;
    if (auto label = m_game->getGui().get<tgui::Label>("PressSpace"))
    {
        float minAlpha = 80.0f;
        float maxAlpha = 230.0f;
        float speed    = 3.5f;

        float sinValue = std::sin(timer * speed);
        float alpha    = minAlpha + ((sinValue + 1.0f) * 0.5f) * (maxAlpha - minAlpha);
        label->getRenderer()->setTextColor(tgui::Color(255, 255, 255, static_cast<unsigned char>(alpha)));
    }
}

void MenuState::draw()
{
    PROFILE_FUNCTION();

    m_game->getWindow().draw(m_backgroundSprite);
}

void MenuState::exit()
{
    auto backgroundMusic = m_gameObjectManager.getGameObject("BackgroundMusic");
    if (backgroundMusic)
        backgroundMusic->getComponent<MusicComponent>()->stopMusic("background");


    PROFILE_FUNCTION();

    m_gameObjectManager.shutdown();
    InputManager::getInstance().unbind("Select", 0);
    m_game->getGui().removeAllWidgets();
    GameState::exit();
}
} // namespace mmt_gd
