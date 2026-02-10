//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once

#include "IComponent.hpp"
#include "ItemComponent.h"
#include "PlayerScoreComponent.h"
#include <vector>
#include <functional>
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
    void     update(float deltaTime) override;
    void     pickup(ItemComponent& pickedUpItem);
    void     loseAllItem();
    void     loseItem();
    void     loseCrown();
    ItemType getItemType()
    {
        if (!holdingItem)
            return ItemType::None;
        return holdingItem->getType();
    }
    bool getGotItemThisFrame()
    {
        return m_gotThisFrame;
    }
    void subscribeToOnPickup(std::function<void()> sub)
    {
        m_onPickup.push_back(sub);
    }
    void subscribeToOnLoseCrown(std::function<void()> sub)
    {
        m_onLoseCrown.push_back(sub);
    }

private:
    void                  HandleCrown(ItemComponent& crownItem);
    ItemComponent*        holdingItem = nullptr;
    ItemComponent*        m_crown     = nullptr;
    PlayerScoreComponent& m_scoreComp;
    float                 m_itemUseTime  = 0;
    bool                  m_gotThisFrame = false;
    std::vector<std::function<void()>> m_onPickup;
    std::vector<std::function<void()>> m_onLoseCrown;
    float                              m_crownMaxCooldown = 1;
    float                              m_crownCooldown = m_crownMaxCooldown;
};
} // namespace mmt_gd
