//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
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
    if (warp)
    {
        m_gameObject.getComponent<RigidBodyComponent>()->getB2Body()->SetTransform(PhysicsManager::s2b(m_spawn), 0);
        for (auto sub : on_respawned)
        {
            sub();
        }
        warp = false;
    }
}

void RespawnComponent::startRespawn()
{
    warp = true;
}
} // namespace mmt_gd
