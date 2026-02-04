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
std::vector<GameObject::Ptr> ItemFactory::createItem(sf::RenderWindow& window, ItemType type, int count, bool canBePickup)
{
    std::vector<GameObject::Ptr> itemGroup;
    for (int i = 0; i < count; i++)
    {
        std::stringstream idStream;
        idStream << "Item_" << static_cast<int>(type) << "_" << i;
        std::string id   = idStream.str();
        auto        item = GameObject::create(id);
        if (type == ItemType::Crown)
            item->setPosition(sf::Vector2f(100, 100));
        else
            item->setPosition(sf::Vector2f(-1000, -1000));
        std::string filePath   = ItemFactory::getAssetPath(type);
        auto        spriteComp = item->addComponent<
                   SpriteRenderComponent>(*item, window, filePath, "GameObjects", ItemFactory::getIntRect(type));
        const auto                      rb             = item->addComponent<RigidBodyComponent>(*item, b2_staticBody);
        auto                            respawn        = item->addComponent<RespawnComponent>(*item);
        std::weak_ptr<RespawnComponent> respawnWeakPtr = respawn;
        auto                            itemComp       = item->addComponent<ItemComponent>(*item, type);
        itemComp->setPickup(canBePickup);
        itemComp->subscribeToOnDisappear(
            [respawnWeakPtr = respawnWeakPtr]()
            {
                if (auto respawnComp = respawnWeakPtr.lock())
                    respawnComp->startRespawn();
            });

        b2PolygonShape polygonShape{};
        tson::Vector2f tsonSize(ItemFactory::getIntRect(type).getSize().x, ItemFactory::getIntRect(type).getSize().y);
        const auto     size = PhysicsManager::t2b(tsonSize);
        polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

        b2FixtureDef fixtureDef{};
        fixtureDef.shape    = &polygonShape;
        fixtureDef.isSensor = true;

        auto collider = item->addComponent<ColliderComponent>(*item, *rb, fixtureDef);

        ItemFactory::addSpecifiedItemComponent(item, *itemComp, type);

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
            break;
        case ItemType::Size:
            path += "Potion.png";
            break;
        case ItemType::Bomb:
            path += "Bomb.png";
            break;
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
            break;
        case ItemType::Size:
            rect = sf::IntRect(0, 0, 16, 32);
            break;
        case ItemType::Bomb:
            rect = sf::IntRect(0, 0, 64, 64);
            break;
    }

    return rect;
}
GameObject::Ptr ItemFactory::addSpecifiedItemComponent(GameObject::Ptr item, ItemComponent& itemComp, ItemType type)
{
    switch (type)
    {
        case mmt_gd::ItemType::None:
            break;
        case mmt_gd::ItemType::Crown:
            break;
        case mmt_gd::ItemType::Bomb:
            break;
        case mmt_gd::ItemType::Size:
            break;
        default:
            break;
    }
    return item;
}
} // namespace mmt_gd