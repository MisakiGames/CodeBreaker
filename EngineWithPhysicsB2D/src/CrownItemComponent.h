#pragma once

#include "ItemComponent.h"
namespace mmt_gd
{
class CrownItemComponent : public ItemComponent
{
public:
    CrownItemComponent(GameObject& gameObject, ItemType type, float maxTime, GameObject& crownSpace) :
    ItemComponent(gameObject, type, maxTime),m_crownSpace(crownSpace) {};
    void use(GameObject& player) override;
    void stopUse(GameObject& player) override;
    void update(float deltaTime);

private:
    sf::Vector2f getRandomPointInCircle(GameObject& player);
    sf::Vector2f getNearestSpaceSpot(sf::Vector2f point, sf::IntRect rect);
    float scaleFactor = 2;
    GameObject& m_crownSpace;
};
} // namespace mmt_gd