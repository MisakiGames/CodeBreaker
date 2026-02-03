#include "stdafx.h"

#include "PickupComponent.h"

namespace mmt_gd
{
void PickupComponent::pickup(ItemComponent& pickedUpItem)
{
    if (pickedUpItem.getType() == ItemType::Crown)
    {
        HandleCrown(pickedUpItem);
        return;
    }
    if (holdingItem)
        return;
    holdingItem = &pickedUpItem;
    pickedUpItem.disappear();
}

void PickupComponent::HandleCrown(ItemComponent& crownItem)
{
    std::cout << "PICKUP" << std::endl;
    m_crown = &crownItem;
    //TODO
}
} // namespace mmt_gd