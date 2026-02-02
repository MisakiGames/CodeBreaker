#include "stdafx.h"

#include "MainState.hpp"

#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "Game.hpp"
#include "InputManager.hpp"
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

    // Create player ship (added to GameObjectManager via GameObjectCreateEvent)
    auto player = PlayerFactory::createPlayer(m_game->getWindow(), PlayerSpawn::TopLeft, m_gameObjectManager);

    // Create enemy ship (added to GameObjectManager via GameObjectCreateEvent)
    auto enemy = ShipFactory::createEnemyShip(m_game->getWindow(), player.get());
    // Moving camera
    {
        const auto camera = GameObject::create("Camera");

        const auto renderComp = camera->addComponent<CameraRenderComponent>(*camera,
                                                                            m_game->getWindow(),
                                                                            m_game->getWindow().getView());

        camera->addComponent<TransformAnimationComponent>(*camera,
                                                          std::make_shared<mmt::TransformAnimationSmoothFollow>(player, 10.F));

        if (!camera->init())
            FF_ERROR_MSG("Could not initialize camera");

        m_gameObjectManager.addGameObject(camera);
        m_spriteManager.setCamera(renderComp.get());
    }

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