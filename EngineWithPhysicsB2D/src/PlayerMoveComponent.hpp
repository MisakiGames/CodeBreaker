#pragma once

#include "DeadComponent.h"
#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
class PlayerMoveComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<PlayerMoveComponent>;

    PlayerMoveComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, DeadComponent& deadComponent, int playerIndex);

    bool init() override;
    void update(float deltaTime) override;

private:
    int                 m_playerIndex;
    DeadComponent&      m_deadComponent;
    RigidBodyComponent& m_rigidBody;
};
} // namespace mmt_gd