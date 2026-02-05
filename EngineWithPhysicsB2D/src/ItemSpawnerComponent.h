#pragma once

#include "IComponent.hpp"
#include "ItemComponent.h"
#include "GameObjectManager.hpp"

#include <random>
#include <unordered_map>
#include <vector>

namespace mmt_gd
{
class ItemSpawnerComponent : public IComponent
{
public:
    ItemSpawnerComponent(GameObject& gameObject) : IComponent(gameObject)
    {
        std::random_device rd;
        randomGen = std::mt19937(rd());
    };
    void LoadItem(sf::RenderWindow& window, ItemType, GameObjectManager& goManager);
    bool init()
    {
        return true;
    }
    void update(float deltaTime);

private:
    void spawnItem();

    bool                                                                   anyItemAbleToPickup();
    std::unordered_map<ItemType, std::vector<std::shared_ptr<GameObject>>> m_items;
    std::unordered_map<ItemType, int>                                      m_itemsIndexPtr;
    float                                                                  m_spawnTime    = 0;
    float                                                                  m_spawnMaxTime = 10;
    std::weak_ptr<ItemComponent>                                           m_lastPickupable;
    std::mt19937                                                           randomGen;
};
} // namespace mmt_gd
