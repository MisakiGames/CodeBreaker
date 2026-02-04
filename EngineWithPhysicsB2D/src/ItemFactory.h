#pragma once
#include "GameObject.hpp"
#include "GameObjectManager.hpp"
#include "ItemComponent.h"

#include <SFML/Graphics.hpp>
#include <vector>
namespace mmt_gd
{
class ItemFactory
{
public:
    static std::vector<GameObject::Ptr> createItem(sf::RenderWindow& window, enum ItemType type, int count, bool canBePickup = true);

private:
    static GameObject::Ptr createBombObject(sf::RenderWindow& );
    static std::string                    getAssetPath(enum ItemType type);
    static sf::IntRect                    getIntRect(enum ItemType type);
    static float                          getMaxTime(enum ItemType type);
    static std::shared_ptr<ItemComponent> addSpecifiedItemComponent(sf::RenderWindow& window,GameObject::Ptr item,
                                                                    enum ItemType                           type);
    static int                            bombCount;
};
}; // namespace mmt_gd
