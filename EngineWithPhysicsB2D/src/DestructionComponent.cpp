#include "stdafx.h"
#include "DestructionComponent.hpp"
#include "GameObject.hpp"
#include "DeadComponent.h"

namespace mmt_gd
{
DestructionComponent::DestructionComponent(GameObject& gameObject, HealthComponent& healthComponent)
    : IComponent(gameObject)
    , m_healthComponent(healthComponent)
    , m_destroyed(false)
{
}

bool DestructionComponent::init()
{
    return true;
}

void DestructionComponent::update(float deltaTime)
{
    if (!m_destroyed && !m_healthComponent.isAlive())
    {
        triggerDestruction();
    }
}



void DestructionComponent::triggerDestruction()
{
    m_destroyed = true;
    m_gameObject.markForDelete();
}
} // namespace mmt_gd
