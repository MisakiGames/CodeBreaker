#include "stdafx.h"

#include "PlayerMoveComponent.hpp"

#include "GameObject.hpp"
#include "InputManager.hpp"

namespace mmt_gd
{
PlayerMoveComponent::PlayerMoveComponent(GameObject&         gameObject,
                                         RigidBodyComponent& rigidBody,
                                         DeadComponent&      deadComponent,
                                         SoundComponent&     soundComponent,
                                         int                 playerIndex) :
IComponent(gameObject),
m_rigidBody(rigidBody),
m_deadComponent(deadComponent),
m_playerIndex(playerIndex),
m_soundComponent(soundComponent)
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

    auto speed            = 300.f;
    auto dashSpeedFactor  = 5.f;
    auto dashCooldownTime = 1.f;

    // Dash logic
    if (m_isDashing && !m_canDash)
    {
        m_isDashing = false;
        for (auto sub : m_onDashEnd)
            sub();
    }

    if (InputManager::getInstance().isKeyDown("dash", m_playerIndex) && m_canDash)
    {
        m_soundComponent.playSound("dash");

        for (auto sub : m_onDash)
            sub();
        m_rigidBody.setVelocity(m_lastMoveDirection * dashSpeedFactor);
        m_isDashing = true;
        m_dashDuration += deltaTime;
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
        m_isDashing = false;
        m_canDash   = false;
        for (auto sub : m_onDashEnd)
            sub();
        return;
    }

    // Normal movement

    if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
    {
        m_lastMoveDirection = sf::Vector2f(speed, 0.f);
        m_rigidBody.setVelocity(m_lastMoveDirection);
    }
    else if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
    {
        m_lastMoveDirection = sf::Vector2f(-speed, 0.f);
        m_rigidBody.setVelocity(m_lastMoveDirection);
    }
    else if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
    {
        m_lastMoveDirection = sf::Vector2f(0.f, -speed);
        m_rigidBody.setVelocity(m_lastMoveDirection);
    }
    else if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
    {
        m_lastMoveDirection = sf::Vector2f(0.f, speed);
        m_rigidBody.setVelocity(m_lastMoveDirection);
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
} // namespace mmt_gd