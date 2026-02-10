//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "GameObject.hpp"

namespace mmt_gd
{
GameObject::~GameObject()
{
    for (const auto& comp : m_componentList)
    {
        comp->setGameObjectDeleted();
    }
    m_componentList.clear();
}
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

bool GameObject::isMarkedForDelete() const
{
    return m_wantToDie;
}

void GameObject::markForDelete()
{
    m_wantToDie = true;
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
