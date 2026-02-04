#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "PhysicsManager.hpp"
#include "SpriteManager.hpp"
#include "PlayerFactory.h"

namespace mmt_gd
{
class FINALFRONTIER_API MainState final : public GameState
{
public:
    MainState(GameStateManager* gameStateManager, Game* game);

    void init() override;
    void exit() override;

    void update(float deltaTime) override;
    void draw() override;

private:
    SpriteManager     m_spriteManager;
    GameObjectManager m_gameObjectManager;
    PhysicsManager    m_physicsManager;

    const float m_maxScore = 100;

    struct PlayerConfig
    {
        PlayerSpawn spawn;
        int         id;
        std::string color;
    };

    std::vector<std::shared_ptr<GameObject>> m_players;
    const std::vector<PlayerConfig> m_playerConfigs = {{PlayerSpawn::TopLeft, 0, "red"},
                                                     {PlayerSpawn::BottomLeft, 1, "blue"},
                                                     {PlayerSpawn::BottomRight, 2, "green"},
                                                     {PlayerSpawn::TopRight, 3, "yellow"}};
};
} // namespace mmt_gd
