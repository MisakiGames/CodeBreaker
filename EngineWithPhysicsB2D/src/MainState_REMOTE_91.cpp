//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "MainState.hpp"

#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "Game.hpp"
#include "InputManager.hpp"
#include "ItemFactory.h"
#include "PlayerFactory.h"
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

    auto player = PlayerFactory::createPlayer(m_game->getWindow(), PlayerSpawn::TopLeft, m_gameObjectManager, 0);
    auto player2 = PlayerFactory::createPlayer(m_game->getWindow(), PlayerSpawn::BottomLeft, m_gameObjectManager, 1);
    auto crown  = ItemFactory::createItem(m_game->getWindow(), ItemType::Crown, m_gameObjectManager, 1);
   
    // Moving camera
    {
        const auto camera = GameObject::create("Camera");
        const auto renderComp = camera->addComponent<CameraRenderComponent>(*camera,
                                                                            m_game->getWindow(),
                                                                            m_game->getWindow().getView());
        renderComp->setTargets({player, player2});
        camera->addComponent<TransformAnimationComponent>(*camera,
                                                          std::make_shared<mmt::TransformAnimationSmoothFollow>(player, 10.F));

        if (!camera->init())
        {
            FF_ERROR_MSG("Could not initialize camera");
        }

        m_gameObjectManager.addGameObject(camera);
        m_spriteManager.setCamera(renderComp.get());
    }

    // Define layer order manually here. Could come from custom file settings.

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
    m_physicsManager.shutdown();
    m_spriteManager.shutdown();
    m_gameObjectManager.shutdown();
}
} // namespace mmt_gd