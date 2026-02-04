#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "PhysicsManager.hpp"
#include "SpriteManager.hpp"

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

    std::vector<std::shared_ptr<GameObject>> m_players;
};
} // namespace mmt_gd
