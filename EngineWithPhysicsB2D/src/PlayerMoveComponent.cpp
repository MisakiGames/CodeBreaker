//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.hpp"

#include "PlayerMoveComponent.hpp"

#include "DamageComponent.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "PickupComponent.hpp"
#include "SpriteAnimationRenderComponent.hpp"

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
    {
        const float deadSpeed = 0.3f;
        m_rigidBody.setVelocity(m_lastMoveDirection * deadSpeed);
        if (m_isDashing)
        {
            m_isDashing = false;
            m_canDash   = false;
            m_damage.setDamage(1);
            for (auto sub : m_onDashEnd)
                sub();
        }
        return;
    }

    auto speed            = 250.f;
    auto dashSpeedFactor  = 5.f;
    auto dashCooldownTime = 0.1f;
    auto dashDamageFactor = 50000.f;

    if (m_dashActive)
    {
        if (InputManager::getInstance().isKeyDown("dash", m_playerIndex) && m_canDash)
        {
            if (!m_isDashing)
            {
                for (auto sub : m_onDash)
                    sub();
            }
            for (auto sub : m_onWhileDash)
                sub();

            m_isDashing = true;
            m_rigidBody.setVelocity(m_lastMoveDirection * dashSpeedFactor);
            m_dashDuration += deltaTime;

            m_damage.setDamage(m_dashDuration * dashDamageFactor + 1);
        }

        if (m_isDashing && (InputManager::getInstance().isKeyReleased("dash", m_playerIndex)))
        {
            m_isDashing = false;
            m_canDash   = false;

            for (auto sub : m_onDashEnd)
                sub();
            return;
        }

        if (!m_canDash && !m_isDashing)
        {
            m_dashCooldown += deltaTime;
            if (m_dashCooldown >= dashCooldownTime && InputManager::getInstance().isKeyUp("dash", m_playerIndex))
            {
                m_canDash      = true;
                m_dashCooldown = 0.f;
                m_dashDuration = 0.f;
                m_damage.setDamage(1); 
            }
        }
    }

    // Normal movement
    if (m_isDashing)
        return;

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
    {
        m_canDash = false;
        m_isDashing = false;

        std::cout << "Damage: " << m_damage.getDamage() << std::endl;

        for (auto sub : m_onDashEnd)
            sub();
    }
}

void PlayerMoveComponent::DoOnMoved()
{
    for (auto sub : m_onMoved)
        sub();
}
} // namespace mmt_gd