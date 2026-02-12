//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "ItemComponent.hpp"
namespace mmt_gd
{
class CrownItemComponent : public ItemComponent
{
public:
    CrownItemComponent(GameObject& gameObject, ItemType type, float maxTime, GameObject& crownSpace) :
    ItemComponent(gameObject, type, maxTime),m_crownSpace(crownSpace) {}
    bool init();
    void use(GameObject& player) override;
    void stopUse(GameObject& player) override;
    void update(float deltaTime);
    sf::Vector2f getCentralPoint();
    void setTouchingCollision(sf::IntRect set)
    {
        m_touchingCollision = set;
    }

private:
    sf::Vector2f getRandomPointInCircle(GameObject& player);
    sf::Vector2f getNearestSpaceSpot(sf::Vector2f point, sf::IntRect rect);
    sf::Vector2f getClosestPointOutside(const sf::IntRect& rect, sf::Vector2f point);
    float scaleFactor = 2;
    GameObject& m_crownSpace;
    sf::IntRect  m_touchingCollision{0, 0, 0, 0};
    sf::Vector2f m_centralPointOffset{0, 0};
};
} // namespace mmt_gd
