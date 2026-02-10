//comment
#pragma once

#include "GameObjectManager.hpp"
#include "IComponent.hpp"
#include "ItemComponent.h"

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
        std::uniform_int_distribution<> distr(m_spawnMinTime, m_spawnMaxTime);
        m_spawnNowTime = distr(randomGen);
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
    float                                                                  m_spawnTime = 0;
    float                                                                  m_spawnNowTime;
    float                                                                  m_spawnMinTime = 10;
    float                                                                  m_spawnMaxTime = 30;
    std::weak_ptr<ItemComponent>                                           m_lastPickupable;
    std::mt19937                                                           randomGen;
};
} // namespace mmt_gd
