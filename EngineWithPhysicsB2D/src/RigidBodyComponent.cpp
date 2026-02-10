//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "RigidBodyComponent.hpp"

#include "DebugDraw.hpp"
#include "EventBus.hpp"
#include "GameObject.hpp"
#include "PhysicsComponentEvents.hpp"
#include "PhysicsManager.hpp"
#include "SpriteRenderComponent.hpp"

#include <Box2D/Box2D.h>

namespace mmt_gd
{
RigidBodyComponent::RigidBodyComponent(GameObject& gameObject, const b2BodyType type, bool allowSleep) :
IComponent(gameObject),
m_body(nullptr)
{
    b2BodyDef def;
    def.position   = PhysicsManager::s2b(gameObject.getPosition());
    def.type       = type;
    def.allowSleep = allowSleep;

    m_body = PhysicsManager::createB2Body(def);

    if (m_body == nullptr)
    {
        sf::err() << "Error creating rigidbody\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<RigidBodyCreateEvent>(*this));
}

RigidBodyComponent::~RigidBodyComponent()
{
    PhysicsManager::destroyB2Body(*m_body);
    m_body = nullptr;
}

void RigidBodyComponent::addVelocity(const sf::Vector2f& velocity) const
{
    auto vel = m_body->GetLinearVelocity();
    vel += PhysicsManager::s2b(velocity);
    m_body->SetLinearVelocity(vel);
}

void RigidBodyComponent::setVelocity(const sf::Vector2f& velocity) const
{
    m_body->SetLinearVelocity(PhysicsManager::s2b(velocity));
}

void RigidBodyComponent::physicsUpdate(float deltaTime)
{
    if (!m_body->IsEnabled())
    {
        return;
    }

    if (m_gameObject.isMarkedForDelete())
    {
        EventBus::getInstance().fireEvent(std::make_shared<RigidBodyDestroyEvent>(*this));
    }

    const auto pos = getB2Body()->GetPosition();

    m_gameObject.setPosition(PhysicsManager::b2s(pos));
    m_gameObject.setRotation(getB2Body()->GetAngle() * 180 / b2_pi);

    //Debug Draw
    auto*  fixture = m_body->GetFixtureList();
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
    aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
    while (fixture != nullptr)
    {
        aabb.Combine(aabb, fixture->GetAABB(0));

        if (auto* const convexShape = fixture->GetShape(); convexShape->GetType() == b2Shape::e_polygon)
        {
            DebugDraw::getInstance().drawPolygonShape(*dynamic_cast<b2PolygonShape*>(convexShape),
                                                      m_gameObject,
                                                      PhysicsManager::s2b(sf::Color::Green));
        }

        fixture = fixture->GetNext();
    }

    DebugDraw::getInstance().drawAabb(&aabb, sf::Color::Green);
    DebugDraw::getInstance().DrawTransform(getB2Body()->GetTransform());
}

b2Body* RigidBodyComponent::getB2Body() const
{
    return m_body;
}
} // namespace mmt_gd
