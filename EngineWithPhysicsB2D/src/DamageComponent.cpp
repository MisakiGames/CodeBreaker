//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "DamageComponent.hpp"

namespace mmt_gd
{
DamageComponent::DamageComponent(GameObject& gameObject, const int damage, const std::string& ownerId) :
IComponent(gameObject),
m_damage(damage),
m_ownerId(ownerId)
{
}

bool DamageComponent::init()
{
    return true;
}

void DamageComponent::update(float deltaTime)
{
}

int DamageComponent::getDamage() const
{
    return m_damage * m_damageFactor;
}

std::string DamageComponent::getOwnerId() const
{
    return m_ownerId;
}

void DamageComponent::setDamageFactor(float set)
{
    m_damageFactor = set;
}

void DamageComponent::setOwnerId(const std::string& ownerId)
{
    m_ownerId = ownerId;
}
} // namespace mmt_gd
