#pragma once

#include "IComponent.hpp"
#include "ItemComponent.h"
#include "PlayerScoreComponent.h"
namespace mmt_gd
{
class PickupComponent : public IComponent
{
public:
    PickupComponent(GameObject& gameObject, PlayerScoreComponent& scoreComp) :
    IComponent(gameObject),
    m_scoreComp(scoreComp) {};
    bool init() override
    {
        return true;
    };
    void update(float deltaTime) override;
    void pickup(ItemComponent& pickedUpItem);
    void loseItem();

private:
    void                  HandleCrown(ItemComponent& crownItem);
    ItemComponent*        holdingItem = nullptr;
    ItemComponent*        m_crown     = nullptr;
    PlayerScoreComponent& m_scoreComp;
};
} // namespace mmt_gd
