#include "stdafx.h"

#include "FollowPlayer.h"

#include "GameObject.hpp"
#include "PhysicsManager.hpp"

namespace mmt_gd
{
void FollowPlayer::update(float deltaTime)
{
    m_gameObject.getComponent<RigidBodyComponent>()
        ->getB2Body()
        ->SetTransform(PhysicsManager::t2b(tson::Vector2f(m_player.getPosition().x, m_player.getPosition().y)), 0);
}
} // namespace mmt_gd