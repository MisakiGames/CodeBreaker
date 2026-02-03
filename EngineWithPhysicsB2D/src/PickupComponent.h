#pragma once

#include "IComponent.hpp"
#include "ItemComponent.h"
namespace mmt_gd
{
class PickupComponent : public IComponent
{
public:
    PickupComponent(GameObject& gameObject) : IComponent(gameObject) {};
    bool init() override
    {
        return true;
    };
    void update(float deltaTime) override {};
    void pickup(ItemComponent& pickedUpItem);

private:
    void           HandleCrown(ItemComponent& crownItem);
    ItemComponent* holdingItem = nullptr;
    ItemComponent* m_crown     = nullptr;
};
} // namespace mmt_gd
