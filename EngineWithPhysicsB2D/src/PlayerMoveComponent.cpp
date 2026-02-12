//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
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
        if (InputManager::getInstance().isKeyDown("dash", m_playerIndex) && m_canDash)
        {
            if (!m_isDashing)
            {
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
            if (m_dashCooldown >= dashCooldownTime && InputManager::getInstance().isKeyUp("dash", m_playerIndex))
            {
                m_canDash      = true;
                m_dashCooldown = 0.f;
                std::cout << "Dash Duration: " << m_dashDuration << "\n";
                m_dashDuration = 0.f;
            }
        }

        if ((InputManager::getInstance().isKeyReleased("dash", m_playerIndex) && m_isDashing) || (m_isDashing && !m_canDash))
        {
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
} // namespace mmt_gd