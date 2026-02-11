//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
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
fixture(nullptr)
{
    def.userData.pointer = reinterpret_cast<uintptr_t>(this);
    fixture              = m_body.getB2Body()->CreateFixture(&def);
}

void ColliderComponent::update(float deltaTime)
{
    //auto box = m_fixture->GetAABB(0);
    //auto ex  = box.GetExtents();
    //DebugDraw::getInstance().drawRectangle(m_gameObject.getPosition(), {ex.x * 2, ex.y * 2}, sf::Color::Green);
}
void ColliderComponent::physicsUpdate(float fDeltaTime)
{
    onCollisionStay();
}

void ColliderComponent::registerOnCollisionEnterFunction(const OnCollisionFunction& func)
{
    m_onCollisionEnterFunctions.push_back(func);
}

void ColliderComponent::registerOnCollisionExitFunction(const OnCollisionFunction& func)
{
    m_onCollisionExitFunctions.push_back(func);
}

void ColliderComponent::registerOnCollisionStayFunction(const OnCollisionFunction& func)
{
    m_onCollisionStayFunctions.push_back(func);
}

void ColliderComponent::onCollisionEnter(ColliderComponent& collider)
{
    for (const auto& f : m_onCollisionEnterFunctions)
    {
        f(*this, collider);
    }
}

void ColliderComponent::onCollisionExit(ColliderComponent& collider)
{
    for (const auto& f : m_onCollisionExitFunctions)
    {
        f(*this, collider);
    }
}
void ColliderComponent::onCollisionStay()
{
    for (const auto touchingCollider : m_touchingCollider)
    {
        for (const auto& f : m_onCollisionStayFunctions)
        {
            f(*this, *touchingCollider);
        }
    }
}
} // namespace mmt_gd