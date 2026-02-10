//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once
#include "IComponent.hpp"

#include <functional>
#include <vector>

namespace mmt_gd
{
enum class ItemType
{
    None,
    Crown,
    Bomb,
    Size
};
class ItemComponent : public IComponent
{
public:
    ItemComponent(GameObject& gameObject, ItemType type, float maxTime);
    virtual bool init() override
    {
        return true;
    };
    virtual void update(float deltaTime) override;
    void         disappear();
    virtual void use(GameObject& player)     = 0;
    virtual void stopUse(GameObject& player) = 0;
    void         subscribeToOnDisappear(std::function<void()> subscriber)
    {
        m_onDisappear.push_back(subscriber);
    };
    ItemType getType()
    {
        return m_type;
    };
    void setPosition(sf::Vector2f setPos);
    void setPickup(bool set)
    {
        m_canBePickedUp = set;
    }
    bool canBePickedUp()
    {
        return m_canBePickedUp;
    }
    float getMaxTime()
    {
        return m_maxTime;
    }

protected:
    bool                               m_canBePickedUp = false;
    ItemType                           m_type;
    std::vector<std::function<void()>> m_onDisappear;
    bool                               warp = false;
    sf::Vector2f                       warpTo{};
    float                              m_maxTime;
};
} // namespace mmt_gd
