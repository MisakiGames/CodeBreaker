//comment
#include "stdafx.h"

#include "PlayerMoveComponent.hpp"

#include "DamageComponent.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "PickupComponent.h"
#include "SpriteAnimationRenderComponent.h"

namespace mmt_gd
{
PlayerMoveComponent::PlayerMoveComponent(GameObject&         gameObject,
                                         RigidBodyComponent& rigidBody,
                                         DeadComponent&      deadComponent,
                                         DamageComponent&    damage,
                                         int                 playerIndex) :
IComponent(gameObject),
m_rigidBody(rigidBody),
m_deadComponent(deadComponent),
m_playerIndex(playerIndex),
m_damage(damage)
{
}

bool PlayerMoveComponent::init()
{
    m_rigidBody.getB2Body()->SetLinearDamping(2.F);

    return true;
}

void PlayerMoveComponent::update(const float deltaTime)
{
    if (m_deadComponent.isDead())
        return;

    auto speed            = 250.f;
    auto dashSpeedFactor  = 5.f;
    auto dashCooldownTime = 0.f;
    if (m_dashActive)
    {
        // Dash logic
        if (m_isDashing && !m_canDash)
        {
            ResizeCollider();
            /*     m_resized   = true;*/
            m_isDashing = false;
            m_damage.setDamageFactor(1);
            for (auto sub : m_onDashEnd)
                sub();
        }

        if (InputManager::getInstance().isKeyDown("dash", m_playerIndex) && m_canDash)
        {
            if (!m_isDashing)
            {
                ResizeCollider();
                for (auto sub : m_onDash)
                    sub();
            }
            for (auto sub : m_onWhileDash)
                sub();
            m_rigidBody.setVelocity(m_lastMoveDirection * dashSpeedFactor);
            m_isDashing = true;
            m_dashDuration += deltaTime;
            m_damage.setDamageFactor(m_dashDuration + 1);
            return;
        }

        if (!m_canDash)
        {
            m_dashCooldown += deltaTime;
            if (m_dashCooldown >= dashCooldownTime)
            {
                m_canDash      = true;
                m_dashCooldown = 0.f;
                std::cout << "Dash Duration: " << m_dashDuration << "\n";
                m_dashDuration = 0.f;
            }
        }

        if (InputManager::getInstance().isKeyReleased("dash", m_playerIndex) && m_isDashing)
        {
            ResizeCollider();
            m_isDashing = false;
            m_canDash   = false;

            m_damage.setDamageFactor(1);
            for (auto sub : m_onDashEnd)
                sub();
            return;
        }
    }

    // Normal movement

    if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
    {
        m_lastMoveDirection = sf::Vector2f(speed, 0.f);
        m_rigidBody.setVelocity(m_lastMoveDirection);
        DoOnMoved();
    }
    else if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
    {
        m_lastMoveDirection = sf::Vector2f(-speed, 0.f);
        m_rigidBody.setVelocity(m_lastMoveDirection);
        DoOnMoved();
    }
    else if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
    {
        m_lastMoveDirection = sf::Vector2f(0.f, -speed);
        m_rigidBody.setVelocity(m_lastMoveDirection);
        DoOnMoved();
    }
    else if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
    {
        m_lastMoveDirection = sf::Vector2f(0.f, speed);
        m_rigidBody.setVelocity(m_lastMoveDirection);
        DoOnMoved();
    }
    else
    {
        m_rigidBody.setVelocity(sf::Vector2f(0.f, 0.f));
        m_lastMoveDirection = sf::Vector2f(0.f, 0.f);
    }
}
void PlayerMoveComponent::OnCollision()
{
    if (m_isDashing)
        m_canDash = false;
}
void PlayerMoveComponent::DoOnMoved()
{
    for (auto sub : m_onMoved)
        sub();
}
void PlayerMoveComponent::ResizeCollider()
{
    //auto item = m_gameObject.getComponent<PickupComponent>();
    //if (item->getItemType() == ItemType::Size && item->getGotItemThisFrame())
    //    return;

    //float yOffsetDivisor = 2;
    //if (m_isDashing)
    //{
    //    yOffsetDivisor = 1 / 1.5;
    //}

    //auto     sprite      = m_gameObject.getComponent<SpriteAnimationRenderComponent>();
    //auto     collider    = m_gameObject.getComponent<ColliderComponent>();
    //auto     fixture     = collider->getFixture();
    //auto     userData    = fixture->GetUserData();
    //b2Filter filter      = fixture->GetFilterData();
    //float    friction    = fixture->GetFriction();
    //float    restitution = fixture->GetRestitution();
    //auto     player_size = sprite->getSprite().getGlobalBounds().getSize();
    //auto     scale       = m_gameObject.getScale();
    //if (m_isDashing)
    //    scale = sf::Vector2f(scale.x, scale.y / 2);
    //else
    //    scale = sf::Vector2f(scale.x + scale.x / 2, scale.y + scale.y / 2);
    //b2PolygonShape shape;
    //auto           newSize = sf::Vector2f(player_size.x * scale.x, player_size.y * scale.y);
    //const auto     size    = PhysicsManager::s2b(newSize);
    //if (m_isDashing)
    //    shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / yOffsetDivisor}, 0);
    //else
    //    shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / yOffsetDivisor - scale.y / 6}, 0);
    //b2FixtureDef fixtureDef;
    //fixtureDef.shape   = &shape;
    //fixtureDef.density = 1.0f;

    //fixtureDef.filter      = filter;
    //fixtureDef.friction    = friction;
    //fixtureDef.restitution = restitution;
    //fixtureDef.userData    = userData;

    //// 4. Swap fixtures
    //collider->getBody().getB2Body()->DestroyFixture(fixture);
    //fixture = collider->getBody().getB2Body()->CreateFixture(&fixtureDef);

    //// 5. Recalculate mass for the new size
    //collider->getBody().getB2Body()->ResetMassData();
    //m_resized = false;
}
} // namespace mmt_gd
