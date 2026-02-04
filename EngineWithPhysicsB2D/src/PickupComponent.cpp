#include "stdafx.h"

#include "PickupComponent.h"

#include "GameObject.hpp"

namespace mmt_gd
{
void PickupComponent::update(float deltaTime)
{
    if (m_crown)
    {
        m_crown->use(m_gameObject);
    }
    if (!holdingItem)
        return;
    m_itemUseTime += deltaTime;
    if (m_itemUseTime >= holdingItem->getMaxTime())
    {
        holdingItem->stopUse(m_gameObject);
        holdingItem = nullptr;
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
    if (holdingItem)
        return;
    holdingItem = &pickedUpItem;
    std::cout << "pickup" << std::endl;
    pickedUpItem.disappear();
    pickedUpItem.setPickup(false);
    holdingItem->use(m_gameObject);
}

void PickupComponent::HandleCrown(ItemComponent& crownItem)
{
    m_crown = &crownItem;
}

void PickupComponent::loseItem()
{
    loseCrown();
    holdingItem   = nullptr;
    m_itemUseTime = 0;
}
void PickupComponent::loseCrown()
{
    if (m_crown)
    {
        m_scoreComp.setHasCrown(false);
        m_crown->disappear();
        m_crown->setPickup(true);
        m_crown->stopUse(m_gameObject);
        m_crown = nullptr;
    }
}
} // namespace mmt_gd