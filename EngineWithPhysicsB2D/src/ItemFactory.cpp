#include "stdafx.h"

#include "ItemFactory.h"

#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "DamageComponent.hpp"
#include "DeadComponent.h"
#include "DestructionComponent.hpp"
#include "EnemyAIComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "GameObjectManager.hpp"
#include "HealthComponent.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "RespawnComponent.h"
#include "RigidBodyComponent.hpp"
#include "SpriteAnimationRenderComponent.h"
#include "SpriteRenderComponent.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationSmoothFollow.hpp"

#include <iostream>
#include <sstream>
#include <string>
namespace mmt_gd
{
std::vector<GameObject::Ptr> ItemFactory::createItem(sf::RenderWindow& window, ItemType type, int count)
{
    std::vector<GameObject::Ptr> itemGroup;
    for (int i = 0; i < count; i++)
    {
        std::stringstream idStream;
        idStream << "Item_" << static_cast<int>(type) << "_" << i;
        std::string id   = idStream.str();
        auto        item = GameObject::create(id);
        item->setPosition(sf::Vector2f(100, 100));
        std::string filePath   = ItemFactory::getAssetPath(type);
        auto        spriteComp = item->addComponent<
                   SpriteRenderComponent>(*item, window, filePath, "GameObjects", ItemFactory::getIntRect(type));
        const auto rb       = item->addComponent<RigidBodyComponent>(*item, b2_staticBody);
        auto       respawn  = item->addComponent<RespawnComponent>(*item);
        auto       itemComp = item->addComponent<ItemComponent>(*item, type);
        itemComp->subscribeToOnDisappear([respawnComp = respawn]() { respawnComp->startRespawn(); });

        b2PolygonShape polygonShape{};
        tson::Vector2f tsonSize(ItemFactory::getIntRect(type).getSize().x, ItemFactory::getIntRect(type).getSize().y);
        const auto     size = PhysicsManager::t2b(tsonSize);
        polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

        b2FixtureDef fixtureDef{};
        fixtureDef.shape    = &polygonShape;
        fixtureDef.isSensor = true;

        auto collider = item->addComponent<ColliderComponent>(*item, *rb, fixtureDef);

        if (!item->init())
        {
            sf::err() << "Could not initialize player ship\n";
        }

        EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(item));

        itemGroup.push_back(item);
    }
    return itemGroup;
}
std::string ItemFactory::getAssetPath(ItemType type)
{
    std::string path = "../Assets/";
    switch (type)
    {
        case ItemType::Crown:
            path += "crown.png";
    }

    return path;
}
sf::IntRect ItemFactory::getIntRect(ItemType type)
{
    sf::IntRect rect = sf::IntRect(0, 0, 32, 32);
    switch (type)
    {
        case ItemType::Crown:
            rect = sf::IntRect(8, 8, 16, 16);
    }

    return rect;
}
} // namespace mmt_gd