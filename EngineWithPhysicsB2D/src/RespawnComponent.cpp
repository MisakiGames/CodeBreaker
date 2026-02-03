#include "stdafx.h"

#include "RespawnComponent.h"

#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
bool RespawnComponent::init()
{
    m_spawn = m_gameObject.getPosition();
    return true;
}

void RespawnComponent::update(float deltaTime)
{
}

void RespawnComponent::startRespawn()
{
    m_gameObject.getComponent<RigidBodyComponent>()->getB2Body()->SetTransform(PhysicsManager::s2b(m_spawn), 0);
    for (auto sub : on_respawned)
    {
        sub();
    }
}
} // namespace mmt_gd