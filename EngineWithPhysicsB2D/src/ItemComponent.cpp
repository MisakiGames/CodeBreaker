#include "stdafx.h"

#include "ItemComponent.h"

#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
ItemComponent::ItemComponent(GameObject& gameObject, ItemType type) : IComponent(gameObject), m_type(type)
{
}

void ItemComponent::disappear()
{
    for (auto sub : m_onDisappear)
        sub();
}

void ItemComponent::setPosition(sf::Vector2f setPos)
{
    m_gameObject.getComponent<RigidBodyComponent>()->getB2Body()->SetTransform(PhysicsManager::s2b(setPos), 0);
}
} // namespace mmt_gd