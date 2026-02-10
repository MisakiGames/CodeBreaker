//comment
#include "stdafx.h"
#include "PhysicsManager.hpp"
#include "VectorAlgebra2D.h"

#include "EnemyAIComponent.hpp"

namespace mmt_gd
{
EnemyAIComponent::EnemyAIComponent(GameObject& gameObject,
                                   RigidBodyComponent& rigidBody,
                                   PlayerShootComponent& shootComponent,
                                   GameObject* playerTarget,
                                   const float moveSpeed,
                                   const float shootInterval,
                                   const float minY,
                                   const float maxY)
    : IComponent(gameObject)
    , m_rigidBody(rigidBody)
    , m_shootComponent(shootComponent)
    , m_playerTarget(playerTarget)
    , m_moveSpeed(moveSpeed)
    , m_shootInterval(shootInterval)
    , m_timeSinceLastShot(0.0f)
    , m_minY(minY)
    , m_maxY(maxY)
    , m_direction(1)
{
}

bool EnemyAIComponent::init()
{
    return true;
}

void EnemyAIComponent::update(const float deltaTime)
{
    updateMovement(deltaTime);
    updateShooting(deltaTime);
}

void EnemyAIComponent::updateMovement(float deltaTime)
{
    const auto currentPos = m_gameObject.getPosition();

    if (currentPos.y <= m_minY && m_direction == -1)
    {
        m_direction = 1;
    }
    else if (currentPos.y >= m_maxY && m_direction == 1)
    {
        m_direction = -1;
    }

    const sf::Vector2f velocity(0.0f, static_cast<float>(m_direction) * m_moveSpeed);
    m_rigidBody.getB2Body()->SetLinearVelocity(PhysicsManager::s2b(velocity));
}

void EnemyAIComponent::updateShooting(const float deltaTime)
{
    m_timeSinceLastShot += deltaTime;

    if (m_timeSinceLastShot >= m_shootInterval)
    {
        m_timeSinceLastShot = 0.0f;

        const sf::Vector2f direction = calculateShootDirection();
        m_shootComponent.shoot(direction);
    }
}

sf::Vector2f EnemyAIComponent::calculateShootDirection() const
{
    if (!m_playerTarget || !m_playerTarget->isActive())
    {
        return sf::Vector2f(-800.0f, 0.0f);
    }

    sf::Vector2f toPlayer = m_playerTarget->getPosition() - m_gameObject.getPosition();
    const float length = MathUtil::length(toPlayer);

    if (length > 0.0f)
    {
        toPlayer = MathUtil::unitVector(toPlayer);
    }

    return toPlayer * 800.0f;
}
} // namespace mmt_gd
