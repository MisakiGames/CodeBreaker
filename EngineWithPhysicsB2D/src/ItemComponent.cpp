#include "stdafx.h"

#include "ItemComponent.h"

#include "GameObject.hpp"

mmt_gd::ItemComponent::ItemComponent(GameObject& gameObject, ItemType type) : IComponent(gameObject), m_type(type)
{
}

void mmt_gd::ItemComponent::disappear()
{
    for (auto sub : m_onDisappear)
        sub();
}