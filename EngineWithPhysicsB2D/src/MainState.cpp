//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.hpp"

#include "MainState.hpp"

#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "Game.hpp"
#include "GameObjectEvents.hpp"
#include "HealthComponent.hpp"
#include "InputManager.hpp"
#include "ItemFactory.hpp"
#include "MusicComponent.hpp"
#include "PickupComponent.hpp"
#include "PlayerFactory.hpp"
#include "PlayerScoreComponent.hpp"
#include "ShipFactory.hpp"
#include "SoundComponent.hpp"
#include "SpriteAnimationRenderComponent.hpp"
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
    const auto    map = tileson.parse(fs::path("../assets/MainMap.tmj"));
    if (map->getStatus() == tson::ParseStatus::OK)
    {
        TileMapLoader::loadTileLayers(map, "../assets", m_spriteManager);
        TileMapLoader::loadObjectLayers(map, "../assets", m_spriteManager, m_gameObjectManager);
    }
    else
    {
#ifdef DEBUG
        sf::err() << "Could not load tile map\n";
#endif
    }

    // Load sounds
    std::string soundPath = "../assets/sounds/";

    // Create players
    for (const auto& config : m_playerConfigs)
    {
        m_players.push_back(
            PlayerFactory::createPlayer(m_game->getWindow(), config.spawn, m_gameObjectManager, config.id, config.color));
    }

    // Create Crown
    auto crown = ItemFactory::createItem(m_game->getWindow(), ItemType::Crown, m_gameObjectManager, 1);

    // Create camera
    m_camera = GameObject::create("Camera");
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

    /* input.bind("up", sf::Keyboard::W, 0);
    input.bind("left", sf::Keyboard::A, 0);
    input.bind("down", sf::Keyboard::S, 0);
    input.bind("right", sf::Keyboard::D, 0);
    input.bind("dash", sf::Keyboard::Enter, 0);*/

    // Load and initialize TGui elements
    m_game->getGui().loadWidgetsFromFile("../assets/mainTgui.txt");
    auto scorePanel  = m_game->getGui().get<tgui::Panel>("ScorePanel");
    auto healthPanel = m_game->getGui().get<tgui::Panel>("HealthPanel");

    for (size_t i = 0; i < m_players.size(); ++i)
    {
        std::string id      = m_players[i]->getId();
        const float opacity = 130;

        if (scorePanel)
        {
            auto panel = tgui::Panel::copy(scorePanel);
            panel->setVisible(true);
            panel->setPosition({tgui::Layout(std::to_string(2 + i * 26) + "%"), "92%"});

            if (auto renderer = panel->getRenderer())
            {
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

            std::string uniqueName = "ScoreUI_" + std::to_string(i);
            m_game->getGui().add(panel, uniqueName);
        }

        if (healthPanel)
        {
            auto panel = tgui::Panel::copy(healthPanel);
            panel->setVisible(true);
            m_game->getGui().add(panel, "HealthUI_" + std::to_string(i));
        }
    }

    m_game->getGui().remove(scorePanel);
    m_game->getGui().remove(healthPanel);

    auto winSound       = GameObject::create("WinSound");
    auto soundComponent = winSound->addComponent<SoundComponent>(*winSound);
    soundComponent->addSound("victory", "../assets/sounds/victory.wav", 100.0f);

    if (!winSound->init())
    {
#ifdef DEBUG
        sf::err() << "Could not initialize sound\n";
#endif
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(winSound));
    auto backgroundMusic = GameObject::create("BackgroundMusic");
    auto musicComponent  = backgroundMusic->addComponent<MusicComponent>(*backgroundMusic);
    musicComponent->addMusic("background", "../assets/musics/bossFight.ogg", 100.0f);

    if (!backgroundMusic->init())
    {
#ifdef DEBUG
        sf::err() << "Could not initialize music\n";
#endif
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(backgroundMusic));

    musicComponent->playMusic("background", true);

    // Define layer order manually here. Could come from custom file settings.
    m_spriteManager.setLayerOrder({"Ground", "GameObjects"});
}

void MainState::update(const float deltaTime)
{
    PROFILE_FUNCTION();

    EventBus::getInstance().processEvents(deltaTime);
    m_gameObjectManager.update(deltaTime);
    m_physicsManager.update(deltaTime);


    if (m_gameEnded)
    {
        m_winTimer += deltaTime;
        if (m_winTimer >= m_winDelay)
            m_gameStateManager->setState("MenuState");

        return;
    }

    for (size_t i = 0; i < m_players.size(); ++i)
    {
        auto  scorePanel  = m_game->getGui().get<tgui::Panel>("ScoreUI_" + std::to_string(i));
        auto  healthPanel = m_game->getGui().get<tgui::Panel>("HealthUI_" + std::to_string(i));
        auto  camComp     = m_camera->getComponent<CameraRenderComponent>();
        auto& player      = m_players[i];

        auto healthComp = player->getComponent<HealthComponent>();
        auto scoreComp  = player->getComponent<PlayerScoreComponent>();
        auto itemComp   = player->getComponent<PickupComponent>();

        auto healthBarOffset = 20.f;

        if (healthPanel && camComp)
        {
            float currentZoom = camComp->getZoom();
            float guiScale    = 1.0f / currentZoom;

            healthPanel->setScale({guiScale, guiScale});

            auto playerWidth = player->getComponent<SpriteAnimationRenderComponent>()->getSprite().getTextureRect().width *
                               player->getScale().x;

            sf::Vector2f worldPos = player->getPosition();
            worldPos.y -= healthBarOffset;
            worldPos.x += (playerWidth / 2.f); 

            sf::Vector2i screenPos = m_game->getWindow().mapCoordsToPixel(worldPos, m_game->getWindow().getView());

            healthPanel->setPosition({static_cast<float>(screenPos.x), static_cast<float>(screenPos.y)});

            if (healthComp)
            {
                if (auto bar = healthPanel->get<tgui::ProgressBar>("Health"))
                    bar->setValue(healthComp->getHealth());
            }
        }

        if (scorePanel && scoreComp)
        {
            if (auto label = scorePanel->get<tgui::Label>("Score"))
            {
                // Show rounded Score
                int roundedScore = static_cast<int>(std::round(scoreComp->getScore()));
                label->setText("Score: \n" + std::to_string(roundedScore) + "%");

                // Check Win Condition
                if (roundedScore >= m_maxScore && !m_gameEnded)
                    endGame(player);
            }
        }

        if (scorePanel && itemComp)
        {
            if (auto itemIcon = scorePanel->get<tgui::Picture>("ItemIcon"))
            {
                ItemType currentType = itemComp->getItemType();
                itemIcon->setVisible(true);

                switch (currentType)
                {
                    case ItemType::Size:
                        itemIcon->getRenderer()->setTexture("../assets/Potion.png");
                        break;
                    case ItemType::Bomb:
                        itemIcon->getRenderer()->setTexture("../assets/Bomb.png");
                        break;
                    default:
                        itemIcon->setVisible(false);
                        break;
                }
            }
        }
    }
}

void MainState::endGame(std::shared_ptr<GameObject> winner)
{
    if (m_gameEnded)
        return;

    m_gameEnded = true;

    // play win sound
    auto winSound = m_gameObjectManager.getGameObject("WinSound");
    if (winSound)
        winSound->getComponent<SoundComponent>()->playSound("victory");
    m_gameEnded = true;

    // deactivate input
    auto& input = InputManager::getInstance();
    for (int i = 0; i < m_players.size(); ++i)
    {
        input.unbind("left", i);
        input.unbind("right", i);
        input.unbind("up", i);
        input.unbind("down", i);
        input.unbind("dash", i);
    }

    // Healthbars ausblenden bei Spielende
    for (size_t i = 0; i < m_players.size(); ++i)
    {
        if (auto healthPanel = m_game->getGui().get<tgui::Panel>("HealthUI_" + std::to_string(i)))
        {
            healthPanel->setVisible(false);
        }
    }

    // focus camera on winning player
    std::vector<std::shared_ptr<GameObject>> winners;
    winners.push_back(winner);
    if (m_camera)
    {
        if (auto camComp = m_camera->getComponent<CameraRenderComponent>())
        {
            camComp->setTargets(winners);
        }
    }
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