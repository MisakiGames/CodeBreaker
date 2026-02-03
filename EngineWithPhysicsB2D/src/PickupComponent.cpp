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
        pickedUpItem.noPickup();
        HandleCrown(pickedUpItem);
        return;
    }
    if (holdingItem)
        return;
    holdingItem = &pickedUpItem;
    pickedUpItem.disappear();
    pickedUpItem.noPickup();
}

void PickupComponent::HandleCrown(ItemComponent& crownItem)
{
    m_crown = &crownItem;
    //TODO
}

void PickupComponent::loseItem()
{
    if (m_crown)
        m_crown->disappear();
    if (holdingItem)
        holdingItem->disappear();
    m_crown     = nullptr;
    holdingItem = nullptr;
}
} // namespace mmt_gd