#include "stdafx.h"

#include "PickupComponent.h"

#include "GameObject.hpp"

namespace mmt_gd
{
void PickupComponent::update(float deltaTime)
{
    const float y_offset = -15;
    const float x_offset = -2.5f;
    if (m_crown)
    {
        auto pos = m_gameObject.getPosition();
        pos      = sf::Vector2f(pos.x + x_offset, pos.y + y_offset);
        m_crown->setPosition(pos);
    }
}
void PickupComponent::pickup(ItemComponent& pickedUpItem)
{
    if (pickedUpItem.getType() == ItemType::Crown)
    {
        pickedUpItem.setPickup(false);
        HandleCrown(pickedUpItem);
        return;
    }
    if (holdingItem != ItemType::None)
        return;
    holdingItem = pickedUpItem.getType();
    pickedUpItem.disappear();
    pickedUpItem.setPickup(false);
}

void PickupComponent::HandleCrown(ItemComponent& crownItem)
{
    m_crown = &crownItem;
    m_scoreComp.setHasCrown(true);
}

void PickupComponent::loseItem()
{
    if (m_crown)
    {
        m_scoreComp.setHasCrown(false);
        m_crown->disappear();
        m_crown->setPickup(true);
        m_crown = nullptr;
    }
    holdingItem = ItemType::None;
}
} // namespace mmt_gd