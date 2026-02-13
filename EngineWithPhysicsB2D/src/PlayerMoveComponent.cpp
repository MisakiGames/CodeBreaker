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
m_baseDamage(damage)
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
            m_rigidBody.setVelocity(m_lastDashDirection * (speed * dashSpeedFactor));
            m_dashDuration += deltaTime;
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
            }
        }
    }

    // Normal movement
    if (m_isDashing)
        return;

    sf::Vector2f movement(0.f, 0.f);

    if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
        movement.x += 1.f;
    if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
        movement.x -= 1.f;
    if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
        movement.y -= 1.f;
    if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
        movement.y += 1.f;

    if (movement != sf::Vector2f(0.f, 0.f))
    {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement /= length;

        m_rigidBody.setVelocity(movement * speed);
        m_lastMoveDirection = movement;

        if (std::abs(movement.x) > std::abs(movement.y))
            m_lastDashDirection = sf::Vector2f(movement.x > 0 ? 1.f : -1.f, 0.f);
        else
            m_lastDashDirection = sf::Vector2f(0.f, movement.y > 0 ? 1.f : -1.f);

        DoOnMoved();
    }
    else
    {
        m_rigidBody.setVelocity(sf::Vector2f(0.f, 0.f));
    }
}

void PlayerMoveComponent::OnCollision()
{
    if (m_isDashing)
    {
        m_canDash   = false;
        m_isDashing = false;

        std::cout << "Damage: " << m_baseDamage.getDamage() << std::endl;

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