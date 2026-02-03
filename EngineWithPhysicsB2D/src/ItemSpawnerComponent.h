#pragma once

#include "IComponent.hpp"
#include "ItemComponent.h"

#include <unordered_map>
#include <vector>

namespace mmt_gd
{
class ItemSpawnerComponent : public IComponent
{
public:
    ItemSpawnerComponent(GameObject& gameObject) : IComponent(gameObject) {};
    void LoadItem(sf::RenderWindow& window, ItemType);
    bool init()
    {
        return true;
    }
    void update(float deltaTime);


private:
    std::unordered_map<ItemType, std::vector<std::shared_ptr<GameObject>>> m_items;
};
} // namespace mmt_gd
