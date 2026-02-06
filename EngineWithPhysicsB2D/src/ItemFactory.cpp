#include "stdafx.h"

#include "ItemFactory.h"

#include "BombAnimationComponent.h"
#include "BombItemComponent.h"
#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "CrownItemComponent.h"
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
#include "ResizeItemComponent.h"
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
int                          ItemFactory::bombCount      = 0;
int                          ItemFactory::crownItemCount = 0;
int                          ItemFactory::bombItemCount  = 0;
int                          ItemFactory::sizeItemCount  = 0;
std::vector<GameObject::Ptr> ItemFactory::createItem(
    sf::RenderWindow&  window,
    ItemType           type,
    GameObjectManager& goManager,
    int                count,
    bool               canBePickup)
{
    std::vector<GameObject::Ptr> itemGroup;
    for (int i = 0; i < count; i++)
    {
        std::stringstream idStream;
        switch (type)
        {
            case mmt_gd::ItemType::Crown:
                idStream << "Item_" << static_cast<int>(type) << "_" << crownItemCount;
                crownItemCount++;
                break;
            case mmt_gd::ItemType::Bomb:
                idStream << "Item_" << static_cast<int>(type) << "_" << bombItemCount;
                bombItemCount++;
                break;
            case mmt_gd::ItemType::Size:
                idStream << "Item_" << static_cast<int>(type) << "_" << sizeItemCount;
                sizeItemCount++;
                break;
        }
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

        auto itemComp = ItemFactory::addSpecifiedItemComponent(window, item, type, goManager);
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

        if (!item->init())
        {
            sf::err() << "Could not initialize item\n";
        }

        EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(item));

        itemGroup.push_back(item);
    }
    return itemGroup;
}
std::string ItemFactory::getAssetPath(ItemType type)
{
    std::string path = "../assets/";
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
            rect = sf::IntRect(20, 20, 28, 34);
            break;
    }

    return rect;
}
std::shared_ptr<ItemComponent> ItemFactory::addSpecifiedItemComponent(
    sf::RenderWindow&  window,
    GameObject::Ptr    item,
    ItemType           type,
    GameObjectManager& goManager)
{
    std::shared_ptr<ItemComponent> itemComp;
    switch (type)
    {
        case mmt_gd::ItemType::Crown:
            itemComp = item->addComponent<CrownItemComponent>(*item, type, 0, *goManager.getGameObject("CrownSpace"));
            itemComp->setPickup(true);
            break;
        case mmt_gd::ItemType::Bomb:
        {
            auto bombItem = item->addComponent<BombItemComponent>(*item, type, 10, ItemFactory::createBombObject(window));
            auto                          soundComponent = item->addComponent<SoundComponent>(*item);
            std::weak_ptr<SoundComponent> soundWeakPtr   = soundComponent;
            soundComponent->addSound("bomb", "../assets/sounds/bomb.wav", 100.0f);
            bombItem->subscribeToOnExplode(
                [soundWeakPtr = soundWeakPtr]()
                {
                    if (auto soundComp = soundWeakPtr.lock())
                        soundComp->playSound("bomb");
                });
            itemComp = bombItem;
        }
        break;
        case mmt_gd::ItemType::Size:
            itemComp = item->addComponent<ResizeItemComponent>(*item, type, 10);
            break;
        default:
            break;
    }
    return itemComp;
}

GameObject::Ptr ItemFactory::createBombObject(sf::RenderWindow& window)
{
    sf::Vector2f      explosionScale(1 / 3.0f, 1 / 3.0f);
    std::stringstream idStream;
    idStream << "Bomb_" << bombCount;
    bombCount++;
    std::string id   = idStream.str();
    auto        bomb = GameObject::create(id);
    bomb->setPosition(sf::Vector2f(-1000, -1000));
    std::string filePath   = "../assets/explosion.png";
    auto        spriteComp = bomb->addComponent<
               BombAnimationComponent>(*bomb, window, filePath, "GameObjects", 6, sf::IntRect(0, 0, 354, 342), sf::Vector2f(7, 1));
    bomb->setScale(explosionScale);
    const auto                      rb             = bomb->addComponent<RigidBodyComponent>(*bomb, b2_staticBody);
    auto                            respawn        = bomb->addComponent<RespawnComponent>(*bomb);
    std::weak_ptr<RespawnComponent> respawnWeakPtr = respawn;
    spriteComp->subscribeToOnFinish(
        [respawnWeakPtr = respawnWeakPtr]()
        {
            if (auto respawn = respawnWeakPtr.lock())
                respawn->startRespawn();
        });

    b2PolygonShape polygonShape{};
    const auto     size = PhysicsManager::t2b(tson::Vector2f(354 * explosionScale.x, 342 * explosionScale.y));
    polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef{};
    fixtureDef.shape    = &polygonShape;
    fixtureDef.isSensor = true;

    auto damage   = bomb->addComponent<DamageComponent>(*bomb, 10, bomb->getId());
    auto collider = bomb->addComponent<ColliderComponent>(*bomb, *rb, fixtureDef);

    if (!bomb->init())
    {
        sf::err() << "Could not initialize bomb\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(bomb));

    return bomb;
}
} // namespace mmt_gd