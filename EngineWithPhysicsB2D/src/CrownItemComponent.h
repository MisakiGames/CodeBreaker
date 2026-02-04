#pragma once

#include "ItemComponent.h"
namespace mmt_gd
{
class CrownItemComponent : public ItemComponent
{
public:
    CrownItemComponent(GameObject& gameObject, ItemType type, float maxTime) :
    ItemComponent(gameObject, type, maxTime) {};
    void use(GameObject& player) override;
    void stopUse(GameObject& player) override;

private:
    float scaleFactor = 2;
};
} // namespace mmt_gd