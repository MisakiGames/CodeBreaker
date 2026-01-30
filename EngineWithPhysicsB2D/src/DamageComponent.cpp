#include "stdafx.h"

#include "DamageComponent.hpp"

namespace mmt_gd
{
DamageComponent::DamageComponent(GameObject& gameObject, const int damage, const std::string& ownerId)
    : IComponent(gameObject)
    , m_damage(damage)
    , m_ownerId(ownerId)
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
    return m_damage;
}

std::string DamageComponent::getOwnerId() const
{
    return m_ownerId;
}

void DamageComponent::setOwnerId(const std::string& ownerId)
{
    m_ownerId = ownerId;
}
} // namespace mmt_gd
