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
    const auto    map = tileson.parse(fs::path("../assets/MainGame.tmj"));
    if (map->getStatus() == tson::ParseStatus::OK)
    {
        TileMapLoader::loadTileLayers(map, "../assets", m_spriteManager);
        TileMapLoader::loadObjectLayers(map, "../assets", m_spriteManager);
    }
    else
    {
        sf::err() << "Could not load tile map\n";
    }

    m_players.push_back(
        PlayerFactory::createPlayer(m_game->getWindow(), PlayerSpawn::TopLeft, m_gameObjectManager, 0, "red"));
    m_players.push_back(
        PlayerFactory::createPlayer(m_game->getWindow(), PlayerSpawn::BottomLeft, m_gameObjectManager, 1, "blue"));
    m_players.push_back(
        PlayerFactory::createPlayer(m_game->getWindow(), PlayerSpawn::BottomRight, m_gameObjectManager, 2, "green"));
    m_players.push_back(
        PlayerFactory::createPlayer(m_game->getWindow(), PlayerSpawn::TopRight, m_gameObjectManager, 3, "yellow"));

    auto crown = ItemFactory::createItem(m_game->getWindow(), ItemType::Crown, m_gameObjectManager, 1);

    // Moving camera
    {
        const auto camera     = GameObject::create("Camera");
        const auto renderComp = camera->addComponent<CameraRenderComponent>(*camera,
                                                                            m_game->getWindow(),
                                                                            m_game->getWindow().getView());
        renderComp->setTargets(m_players);

        if (!camera->init())
        {
            FF_ERROR_MSG("Could not initialize camera");
        }

        m_gameObjectManager.addGameObject(camera);
        m_spriteManager.setCamera(renderComp.get());
    }

    // Set Player Input Actions
    // May move to view later on
    InputManager::getInstance().bind("up", sf::Keyboard::W, 0);
    InputManager::getInstance().bind("left", sf::Keyboard::A, 0);
    InputManager::getInstance().bind("down", sf::Keyboard::S, 0);
    InputManager::getInstance().bind("right", sf::Keyboard::D, 0);
    InputManager::getInstance().bind("dash", sf::Keyboard::Enter, 0);

    InputManager::getInstance().bind("left", {sf::Joystick::X, 50.0f, false}, 1);
    InputManager::getInstance().bind("right", {sf::Joystick::X, 50.0f, true}, 1);
    InputManager::getInstance().bind("up", {sf::Joystick::Y, 50.0f, false}, 1);
    InputManager::getInstance().bind("down", {sf::Joystick::Y, 50.0f, true}, 1);
    InputManager::getInstance().bind("dash", 0, 1);

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

        playerPanel->setPosition({tgui::Layout(std::to_string(2 + i * 24) + "%"), "85%"});

        auto renderer = playerPanel->getRenderer();
        if (renderer)
        {
            const float opacity = 130;
            std::string id = m_players[i]->getId();

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

    // Define layer order manually here. Could come from custom file settings.
    m_spriteManager.setLayerOrder({"Ground", "GameObjects"});
}

void MainState::update(const float deltaTime)
{
    PROFILE_FUNCTION();
    if (InputManager::getInstance().isKeyPressed("Exit"))
    {
        m_gameStateManager->setState("MenuState");
        return;
    }

    // UI Update
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

    for (int i = 0; i < InputManager::getInstance().getPlayerCount(); ++i)
    {
        // Update all Uis with their player data
    }

    EventBus::getInstance().processEvents(deltaTime);
    m_gameObjectManager.update(deltaTime);
    m_physicsManager.update(deltaTime);
}

void MainState::draw()
{
    PROFILE_FUNCTION();
    m_spriteManager.draw();
}

void MainState::exit()
{
    PROFILE_FUNCTION();
    m_game->getGui().removeAllWidgets();
    m_physicsManager.shutdown();
    m_spriteManager.shutdown();
    m_gameObjectManager.shutdown();
}
} // namespace mmt_gd