#include "stdafx.h"

#include "ItemSpawnerComponent.h"

#include "GameObject.hpp"
#include "ItemFactory.h"

namespace mmt_gd
{

void ItemSpawnerComponent::LoadItem(sf::RenderWindow& window, ItemType type)
{
    m_items.emplace(type, ItemFactory::createItem(window, type, 10));
}

void ItemSpawnerComponent::update(float deltaTime)
{
    //TODO
}

} // namespace mmt_gd