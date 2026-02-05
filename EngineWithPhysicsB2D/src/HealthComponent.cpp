#include "stdafx.h"

#include "HealthComponent.hpp"

namespace mmt_gd
{
HealthComponent::HealthComponent(GameObject& gameObject, const int maxHealth, float maxInvince, const bool invincible) :
IComponent(gameObject),
m_maxHealth(maxHealth),
m_currentHealth(maxHealth),
m_invincible(invincible),
m_maxInvinceTime(maxInvince)
{
}

bool HealthComponent::init()
{
    return true;
}

void HealthComponent::update(float deltaTime)
{
    m_currentHealth -= m_damagePerSec * deltaTime;
    if (m_invincible)
    {
        m_invinceTime += deltaTime;
        if (m_invinceTime >= m_maxInvinceTime)
        {
            m_invincible  = false;
            m_invinceTime = 0;
        }
    }
    else
    {
        m_invinceTime = 0;
    }
}

void HealthComponent::takeDamage(const int damage)
{
    if (m_invincible)
    {
        return;
    }

    m_currentHealth -= damage;
    if (m_currentHealth < 0)
    {
        m_currentHealth = 0;
    }
    setInvincible(true);
}

void HealthComponent::setDamagePerSecond(float damage)
{
    m_damagePerSec = damage;
}

void HealthComponent::heal(const int amount)
{
    m_currentHealth += amount;
    if (m_currentHealth > m_maxHealth)
    {
        m_currentHealth = m_maxHealth;
    }
}

bool HealthComponent::isAlive() const
{
    return m_currentHealth > 0;
}

bool HealthComponent::isInvincible() const
{
    return m_invincible;
}

int HealthComponent::getHealth() const
{
    return m_currentHealth;
}

int HealthComponent::getMaxHealth() const
{
    return m_maxHealth;
}

void HealthComponent::setInvincible(const bool invincible)
{
    m_invincible = invincible;
}
} // namespace mmt_gd