//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
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
    static std::vector<GameObject::Ptr> createItem(
        sf::RenderWindow& window,
        enum ItemType     type,
        GameObjectManager& goManager,
        int               count,
        bool              canBePickup = true);

private:
    static GameObject::Ptr                createBombObject(sf::RenderWindow&);
    static std::string                    getAssetPath(enum ItemType type);
    static sf::IntRect                    getIntRect(enum ItemType type);
    static std::shared_ptr<ItemComponent> addSpecifiedItemComponent(sf::RenderWindow& window,
                                                                    GameObject::Ptr   item,
        enum ItemType     type,
        GameObjectManager& goManager);
    static int                            bombCount;
    static int                            crownItemCount;
    static int                            bombItemCount;
    static int                            sizeItemCount;
};
}; // namespace mmt_gd
