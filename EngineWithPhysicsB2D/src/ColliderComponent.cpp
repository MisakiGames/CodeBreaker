#include "stdafx.h"

#include "ColliderComponent.hpp"

#include "DebugDraw.hpp"
#include "EventBus.hpp"
#include "GameObject.hpp"
#include "PhysicsComponentEvents.hpp"
#include "PhysicsManager.hpp"

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
    if (m_needScale)
        scale();
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
void ColliderComponent::setScale(sf::Vector2f scale)
{
    m_scale     = scale;
    m_needScale = true;
}
void ColliderComponent::scale()
{
    m_needScale          = false;
    auto     userData    = m_fixture->GetUserData();
    b2Filter filter      = m_fixture->GetFilterData();
    float    friction    = m_fixture->GetFriction();
    float    restitution = m_fixture->GetRestitution();

    b2PolygonShape shape;
    auto           newSize = sf::Vector2f(m_size.x * m_scale.x, m_size.y * m_scale.y);
    const auto     size    = PhysicsManager::s2b(newSize);
    shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape   = &shape;
    fixtureDef.density = 1.0f;

    fixtureDef.filter      = filter;
    fixtureDef.friction    = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.userData    = userData;

    // 4. Swap fixtures
    m_body.getB2Body()->DestroyFixture(m_fixture);
    m_fixture = m_body.getB2Body()->CreateFixture(&fixtureDef);

    // 5. Recalculate mass for the new size
    m_body.getB2Body()->ResetMassData();
}
} // namespace mmt_gd