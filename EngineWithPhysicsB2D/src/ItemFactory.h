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
    static std::vector<GameObject::Ptr> createItem(sf::RenderWindow&  window,
                                                     enum ItemType      type,
                                                     
                                                     int                count);

    private:
    static std::string getAssetPath(enum ItemType type);
    static sf::IntRect getIntRect(enum ItemType type);
};
}; // namespace mmt_gd
