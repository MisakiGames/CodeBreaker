#include "stdafx.h"

#include "GameObject.hpp"

namespace mmt_gd
{
void GameObject::removeComponent(const IComponent::Ptr& component)
{
    m_componentList.remove(component);
}

void GameObject::update(const float deltaTime) const
{
    for (const auto& comp : m_componentList)
    {
        comp->update(deltaTime);
    }
}

bool GameObject::init() const
{
    for (const auto& comp : m_componentList) // NOLINT(readability-use-anyofallof)
    {
        if (!comp->init())
        {
            sf::err() << "Could not initialize component of object " << m_id << '\n';
            return false;
        }
    }
    return true;
}
} // namespace mmt_gd
