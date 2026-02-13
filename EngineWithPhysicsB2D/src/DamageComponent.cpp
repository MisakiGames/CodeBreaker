//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.hpp"

#include "DamageComponent.hpp"

namespace mmt_gd
{
DamageComponent::DamageComponent(GameObject& gameObject, const float damage, const std::string& ownerId) :
IComponent(gameObject),
m_baseDamage(damage),
m_ownerId(ownerId)
{
}

bool DamageComponent::init()
{
    return true;
}

void DamageComponent::update(float deltaTime)
{
    for (auto removeMultiply : m_multipliersToRemove)
    {
        m_multipliers.remove(removeMultiply);
    }
    m_multipliersToRemove = m_multipliers;
}

int DamageComponent::getDamage()
{
    float m_damage = m_baseDamage;
    for (auto multiply : m_multipliers)
        m_damage *= multiply;
    return m_damage;
}

std::string DamageComponent::getOwnerId() const
{
    return m_ownerId;
}

void DamageComponent::setBaseDamage(float damage)
{
    m_baseDamage = damage;
}

void DamageComponent::setOwnerId(const std::string& ownerId)
{
    m_ownerId = ownerId;
}
} // namespace mmt_gd