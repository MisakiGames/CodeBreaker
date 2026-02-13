//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.hpp"

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
    auto temp    = m_invincible;
    m_invincible = false;
    takeDamageSecretly(m_damagePerSec * deltaTime);
    m_invincible = temp;
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

void HealthComponent::takeDamage(const float damage)
{
    if (m_invincible || damage == 0)
    {
        return;
    }
    std::cout << "CurrentHealth: " << m_currentHealth << std::endl;
    std::cout << "Damage: " << damage << std::endl;
    m_currentHealth -= damage;
    std::cout << "CurrentHealthAfterDamage: " << m_currentHealth << std::endl;
    if (m_currentHealth <= 0)
    {
        m_currentHealth = 0;
    }

    setInvincible(true);
    for (auto sub : m_onTakeDamage)
        sub();
}

void HealthComponent::takeDamageSecretly(const float damage)
{
    m_currentHealth -= damage;
    if (m_currentHealth < 0)
    {
        m_currentHealth = 0;
    }
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

void HealthComponent::healPercent(float percent)
{
    percent = percent / 100;
    heal(m_maxHealth * percent);
}

void HealthComponent::setInvincible(const bool invincible)
{
    m_invincible = invincible;
}
} // namespace mmt_gd