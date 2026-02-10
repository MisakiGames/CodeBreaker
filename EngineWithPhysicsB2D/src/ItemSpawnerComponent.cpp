//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "ItemSpawnerComponent.h"

#include "GameObject.hpp"
#include "ItemFactory.h"

namespace mmt_gd
{
void ItemSpawnerComponent::LoadItem(sf::RenderWindow& window, ItemType type, GameObjectManager& goManager)
{
    m_items.emplace(type, ItemFactory::createItem(window, type, goManager, 10, false));
    m_itemsIndexPtr.emplace(type, 0);
}

void ItemSpawnerComponent::update(float deltaTime)
{
    if (!m_lastPickupable.expired())
    {
        if (auto lastPickupable = m_lastPickupable.lock(); lastPickupable->canBePickedUp())
            return;
        m_lastPickupable.reset();
    }
    if (anyItemAbleToPickup())
        return;
    m_spawnTime += deltaTime;
    if (m_spawnTime >= m_spawnNowTime)
    {
        spawnItem();
        m_spawnTime = 0;
    }
}
void ItemSpawnerComponent::spawnItem()
{
    std::uniform_int_distribution<> distr(0, m_items.size() - 1);
    auto                            randomNum = distr(randomGen);
    auto                            it        = m_items.begin();
    std::advance(it, randomNum);
    ItemType choseType = it->first;
    auto     item      = m_items[choseType][m_itemsIndexPtr[choseType]]->getComponent<ItemComponent>();
    item->setPosition(m_gameObject.getPosition());
    item->setPickup(true);
    m_itemsIndexPtr[choseType]++;
    if (m_itemsIndexPtr[choseType] == m_items[choseType].size())
        m_itemsIndexPtr[choseType] = 0;
    std::uniform_int_distribution<> distrRandomSpawn(m_spawnMinTime, m_spawnMaxTime);
    m_spawnNowTime = distrRandomSpawn(randomGen);
}
bool ItemSpawnerComponent::anyItemAbleToPickup()
{
    bool ablePickup = false;
    for (auto itemVector : m_items)
    {
        for (auto item : itemVector.second)
        {
            auto itemComp = item->getComponent<ItemComponent>();
            if (itemComp->canBePickedUp())
            {
                m_lastPickupable = itemComp;
                ablePickup       = true;
                break;
            }
        }
        if (ablePickup)
            break;
    }

    return ablePickup;
}
} // namespace mmt_gd
