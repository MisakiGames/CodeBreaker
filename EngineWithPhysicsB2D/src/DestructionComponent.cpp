#include "stdafx.h"

#include "DestructionComponent.hpp"

#include "DeadComponent.h"
#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
DestructionComponent::DestructionComponent(GameObject& gameObject, HealthComponent& healthComponent) :
IComponent(gameObject),
m_healthComponent(healthComponent),
m_destroyed(false)
{
}

bool DestructionComponent::init()
{
    return true;
}

void DestructionComponent::update(float deltaTime)
{
    //we act like its destroyed when its yeetet to notherness :3
    if (!m_destroyed && !m_healthComponent.isAlive())
    {
        setPosition(sf::Vector2f(-1500, -1500));
    }
    if (warp)
        m_gameObject.getComponent<RigidBodyComponent>()->getB2Body()->SetTransform(PhysicsManager::s2b(warpTo), 0);
    warp = false;
}
void DestructionComponent::setPosition(sf::Vector2f setPos)
{
    warpTo = setPos;
    warp   = true;
}
} // namespace mmt_gd