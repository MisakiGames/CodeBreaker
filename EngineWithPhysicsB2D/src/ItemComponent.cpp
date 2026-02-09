#include "stdafx.h"

#include "ItemComponent.h"

#include "ColliderComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
ItemComponent::ItemComponent(GameObject& gameObject, ItemType type, float maxTime) :
IComponent(gameObject),
m_type(type),
m_maxTime(maxTime)
{
}

void ItemComponent::update(float deltaTime)
{
    if (warp)
    {
        m_gameObject.getComponent<RigidBodyComponent>()->getB2Body()->SetTransform(PhysicsManager::s2b(warpTo), 0);
        auto collider = m_gameObject.getComponent<ColliderComponent>();
        collider->getFixture()->Refilter();
        warp = false;
    }
}

void ItemComponent::disappear()
{
    for (auto sub : m_onDisappear)
        sub();
}

void ItemComponent::setPosition(sf::Vector2f setPos)
{
    warpTo = setPos;
    warp   = true;
}
} // namespace mmt_gd