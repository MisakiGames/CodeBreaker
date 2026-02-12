//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "MainState.hpp"

#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "Game.hpp"
#include "HealthComponent.hpp"
#include "InputManager.hpp"
#include "ItemFactory.h"
#include "PlayerFactory.h"
#include "PlayerScoreComponent.h"
#include "ShipFactory.hpp"
#include "TileMapLoader.hpp"
#include "Tileson.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationSmoothFollow.hpp"

#include <memory>
#include <thread>
#include "SoundComponent.hpp"
#include "GameObjectEvents.hpp"
#include "MusicComponent.h"

namespace mmt_gd
{
MainState::MainState(GameStateManager* gameStateManager, Game* game) :
GameState(gameStateManager, game),
m_spriteManager(game->getWindow())
{
}

void MainState::init()
{
    PROFILE_FUNCTION();

    m_gameObjectManager.init();
    m_spriteManager.init();
    m_physicsManager.init();

    // Load tile map
    tson::Tileson tileson;
    const auto    map = tileson.parse(fs::path("../assets/MainMap.tmj"));
    if (map->getStatus() == tson::ParseStatus::OK)
    {
        TileMapLoader::loadTileLayers(map, "../assets", m_spriteManager);
        TileMapLoader::loadObjectLayers(map, "../assets", m_spriteManager, m_gameObjectManager);
    }
    else
    {
        sf::err() << "Could not load tile map\n";
    }

    // Load sounds
    std::string soundPath = "../assets/sounds/";


    // Create players
    for (const auto& config : m_playerConfigs)
    {
        m_players.push_back(
            PlayerFactory::createPlayer(
                m_game->getWindow(),
                config.spawn,
                m_gameObjectManager,
                config.id, 
                config.color));
    }
    auto crown = ItemFactory::createItem(m_game->getWindow(), ItemType::Crown, m_gameObjectManager, 1);
    m_camera   = GameObject::create("Camera");
    {
        const auto renderComp = m_camera->addComponent<CameraRenderComponent>(*m_camera,
                                                                              m_game->getWindow(),
                                                                              m_game->getWindow().getView());
        renderComp->setTargets(m_players);

        if (!m_camera->init())
        {
            FF_ERROR_MSG("Could not initialize camera");
        }

        m_gameObjectManager.addGameObject(m_camera);
        m_spriteManager.setCamera(renderComp.get());
    }

    auto& input = InputManager::getInstance();

    for (int i = 0; i < m_players.size(); ++i)
    {
        input.bind("left", {sf::Joystick::X, 50.0f, false}, i);
        input.bind("right", {sf::Joystick::X, 50.0f, true}, i);
        input.bind("up", {sf::Joystick::Y, 50.0f, false}, i);
        input.bind("down", {sf::Joystick::Y, 50.0f, true}, i);
        input.bind("dash", 0u, i);
    }

    input.bind("up", sf::Keyboard::W, 0);
    input.bind("left", sf::Keyboard::A, 0);
    input.bind("down", sf::Keyboard::S, 0);
    input.bind("right", sf::Keyboard::D, 0);
    input.bind("dash", sf::Keyboard::Enter, 0);

    // Load and initialize TGui elements
    m_game->getGui().loadWidgetsFromFile("../assets/mainTgui.txt");
    auto panel = m_game->getGui().get<tgui::Panel>("Player");

    if (!panel)
        return;

    for (size_t i = 0; i < m_players.size(); ++i)
    {
        auto playerPanel = tgui::Panel::copy(panel);
        playerPanel->setVisible(true);

        std::string uniqueName = "PlayerUI_" + std::to_string(i);
        m_game->getGui().add(playerPanel, uniqueName);

        playerPanel->setPosition({tgui::Layout(std::to_string(2 + i * 26) + "%"), "86%"});

        auto renderer = playerPanel->getRenderer();
        if (renderer)
        {
            const float opacity = 130;
            std::string id      = m_players[i]->getId();

            if (id == "Player_red")
                renderer->setBackgroundColor(tgui::Color(255, 0, 0, opacity));
            else if (id == "Player_blue")
                renderer->setBackgroundColor(tgui::Color(0, 0, 255, opacity));
            else if (id == "Player_green")
                renderer->setBackgroundColor(tgui::Color(0, 255, 0, opacity));
            else if (id == "Player_yellow")
                renderer->setBackgroundColor(tgui::Color(255, 255, 0, opacity));
            else
                renderer->setBackgroundColor(tgui::Color(100, 100, 100, opacity));
        }
    }
    m_game->getGui().remove(panel);

      auto winSound = GameObject::create("WinSound");
    auto soundComponent = winSound->addComponent<SoundComponent>(*winSound);
      soundComponent->addSound("victory", "../assets/sounds/victory.wav", 100.0f);

          if (!winSound->init())
      {
          sf::err() << "Could not initialize sound\n";
      }

      EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(winSound));
      auto backgroundMusic= GameObject::create("BackgroundMusic");
      auto musicComponent  = backgroundMusic->addComponent<MusicComponent>(*backgroundMusic);
      musicComponent->addMusic("background", "../assets/musics/bossFight.ogg", 100.0f);

      if (!backgroundMusic->init())
      {
          sf::err() << "Could not initialize music\n";
      }

      EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(backgroundMusic));

      musicComponent->playMusic("background",true);

    // Define layer order manually here. Could come from custom file settings.
    m_spriteManager.setLayerOrder({"Ground", "GameObjects"});
}

void MainState::update(const float deltaTime)
{
    PROFILE_FUNCTION();

    for (size_t i = 0; i < m_players.size(); ++i)
    {
        auto panel = m_game->getGui().get<tgui::Panel>("PlayerUI_" + std::to_string(i));
        if (!panel)
            continue;

        auto& player = m_players[i];

        if (auto scoreComp = player->getComponent<PlayerScoreComponent>())
        {
            if (auto label = panel->get<tgui::Label>("Score"))
            {
                int roundedScore = static_cast<int>(std::round(scoreComp->getScore()));
                label->setText("Score: \n" + std::to_string(roundedScore) + "%");

                if (roundedScore >= m_maxScore)
                {
                    if (!m_gameEnded)
                        endGame(m_players[i]);

                    m_camera->update(deltaTime);
                    m_winTimer += deltaTime;

                    if (m_winTimer < m_winDelay)
                        return;

                    m_gameStateManager->setState("MenuState");
                    return;
                }
            }

            if (auto crownImg = panel->get<tgui::Picture>("Crown"))
                crownImg->setVisible(scoreComp->hasCrown());
        }

        if (auto healthComp = player->getComponent<HealthComponent>())
        {
            if (auto bar = panel->get<tgui::ProgressBar>("Health"))
                bar->setValue(healthComp->getHealth());
        }
    }

    EventBus::getInstance().processEvents(deltaTime);
    m_gameObjectManager.update(deltaTime);
    m_physicsManager.update(deltaTime);
}

void MainState::endGame(std::shared_ptr<GameObject> winner)
{
    auto winSound= m_gameObjectManager.getGameObject("WinSound");
    if (winSound)
        winSound->getComponent<SoundComponent>()->playSound("victory");
    m_gameEnded = true;

    //deactivate input
    auto& input = InputManager::getInstance();
    for (int i = 1; i < m_players.size(); ++i)
    {
        input.unbind("left", i);
        input.unbind("right", i);
        input.unbind("up", i);
        input.unbind("down", i);
        input.unbind("dash", i);
    }

    std::vector<std::shared_ptr<GameObject>> winners;
    winners.push_back(winner);
    m_camera->getComponent<CameraRenderComponent>()->setTargets(winners);
}

void MainState::draw()
{
    PROFILE_FUNCTION();
    m_spriteManager.draw();
}

void MainState::exit()
{
    auto backgroundMusic = m_gameObjectManager.getGameObject("BackgroundMusic");
    if (backgroundMusic)
        backgroundMusic->getComponent<MusicComponent>()->stopMusic("background");

    PROFILE_FUNCTION();

    m_game->getGui().removeAllWidgets();
    m_physicsManager.shutdown();
    m_spriteManager.shutdown();
    m_gameObjectManager.shutdown();
    m_players.clear();
    m_camera    = nullptr;
    m_gameEnded = false;
    m_winTimer  = 0.0f;
}
} // namespace mmt_gd
