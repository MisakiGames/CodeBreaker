//comment
#include "stdafx.h"

#include "PickupComponent.h"

#include "GameObject.hpp"

namespace mmt_gd
{
void PickupComponent::update(float deltaTime)
{
    m_gotThisFrame = false;
    if (m_crown)
    {
        m_crown->use(m_gameObject);
    }
    else
    {
        m_crownCooldown += deltaTime;
    }
    if (!holdingItem)
        return;
    m_itemUseTime += deltaTime;
    if (m_itemUseTime >= holdingItem->getMaxTime())
    {
        loseItem();
    }
}
void PickupComponent::pickup(ItemComponent& pickedUpItem)
{
    if (pickedUpItem.getType() == ItemType::Crown)
    {
        if (m_crownCooldown < m_crownMaxCooldown)
            return;
        pickedUpItem.setPickup(false);
        for (auto sub : m_onPickup)
            sub();
        HandleCrown(pickedUpItem);
        return;
    }
    if (holdingItem)
        return;
    for (auto sub : m_onPickup)
        sub();
    holdingItem = &pickedUpItem;
    std::cout << "pickup" << std::endl;
    pickedUpItem.disappear();
    pickedUpItem.setPickup(false);
    holdingItem->use(m_gameObject);
    m_gotThisFrame = true;
}

void PickupComponent::HandleCrown(ItemComponent& crownItem)
{
    m_crown = &crownItem;
}

void PickupComponent::loseAllItem()
{
    loseCrown();
    loseItem();
}
void PickupComponent::loseItem()
{
    if (holdingItem)
    {
        holdingItem->stopUse(m_gameObject);
        holdingItem = nullptr;
    m_itemUseTime = 0;
    }
}
void PickupComponent::loseCrown()
{
    if (m_crown)
    {
        m_crownCooldown = 0;
        m_scoreComp.setHasCrown(false);
        m_crown->stopUse(m_gameObject);
        m_crown->setPickup(true);
        m_crown = nullptr;
        for (auto sub : m_onLoseCrown)
            sub();
    }
}
} // namespace mmt_gd
