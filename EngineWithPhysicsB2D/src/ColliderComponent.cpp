#include "stdafx.h"

#include "ColliderComponent.hpp"

#include "DebugDraw.hpp"
#include "EventBus.hpp"
#include "GameObject.hpp"
#include "PhysicsComponentEvents.hpp"

namespace mmt_gd
{
ColliderComponent::ColliderComponent(GameObject& gameObject, RigidBodyComponent& body, b2FixtureDef& def) :
IComponent(gameObject),
m_body(body),
m_fixture(nullptr)
{
    def.userData.pointer = reinterpret_cast<uintptr_t>(this);
    m_fixture            = m_body.getB2Body()->CreateFixture(&def);
}

void ColliderComponent::update(float deltaTime)
{
    //auto box = m_fixture->GetAABB(0);
    //auto ex  = box.GetExtents();
    //DebugDraw::getInstance().drawRectangle(m_gameObject.getPosition(), {ex.x * 2, ex.y * 2}, sf::Color::Green);
}

void ColliderComponent::registerOnCollisionFunction(const OnCollisionFunction& func)
{
    m_onCollisionFunctions.push_back(func);
}

void ColliderComponent::onCollision(ColliderComponent& collider)
{
    for (const auto& f : m_onCollisionFunctions)
    {
        f(*this, collider);
    }
}
} // namespace mmt_gd
