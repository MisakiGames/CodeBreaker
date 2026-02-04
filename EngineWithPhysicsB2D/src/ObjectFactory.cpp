#include "stdafx.h"

#include "ObjectFactory.hpp"

#include "ColliderComponent.hpp"
#include "DamageComponent.hpp"
#include "DestructionComponent.hpp"
#include "GameObjectEvents.hpp"
#include "ItemSpawnerComponent.h"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "Tileson.hpp"

#include <iostream>
#include <vector>

namespace mmt_gd
{
static GameObject::Ptr loadSprite(tson::Object&        object,
                                  const std::string&   layer,
                                  const fs::path&      resourcePath,
                                  const SpriteManager& spriteManager)
{
    auto gameObject = GameObject::create(object.getName());

    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));

    // Parse data from file
    sf::IntRect textureRect{};
    textureRect.width  = object.getSize().x;
    textureRect.height = object.getSize().y;
    fs::path              spriteTexture;
    bool                  destroyable = false;
    float                 health      = 0;
    bool                  spawner     = false;
    std::vector<ItemType> itemType;

    for (const auto* property : object.getProperties().get())
    {
        if (auto name = property->getName(); name == "Texture")
        {
            spriteTexture = resourcePath / std::any_cast<std::string>(property->getValue());
        }
        else if (name == "TextureRectLeft")
        {
            textureRect.left = std::any_cast<int>(property->getValue());
        }
        else if (name == "TextureRectTop")
        {
            textureRect.top = std::any_cast<int>(property->getValue());
        }
        else if (name == "Mass")
        {
            auto mass = std::any_cast<float>(property->getValue());
        }
        else if (name == "Destroyable")
        {
            destroyable = std::any_cast<bool>(property->getValue());
        }
        else if (name == "Health")
        {
            health = std::any_cast<float>(property->getValue());
        }
        else if (name == "Spawner")
        {
            spawner = std::any_cast<bool>(property->getValue());
        }
        else if (name == "ItemType")
        {
            itemType.push_back(static_cast<ItemType>(std::any_cast<int>(property->getValue())));
        }
    }

    if (spriteTexture.string().length() > 0)
    {
        const auto renderComp = gameObject->addComponent<SpriteRenderComponent>(*gameObject,
                                                                                spriteManager.getWindow(),
                                                                                spriteTexture.string(),
                                                                                layer);

        renderComp->getSprite().setTextureRect(textureRect);
    }

    const auto rigidComp = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_staticBody);

    if (destroyable)
    {
        auto health = gameObject->addComponent<HealthComponent>(*gameObject, 1, false);
        gameObject->addComponent<DestructionComponent>(*gameObject, *health);

        b2PolygonShape polygonShape;
        const auto     size = PhysicsManager::t2b(object.getSize(), true);
        polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);
        b2FixtureDef fixtureDef{};
        fixtureDef.shape   = &polygonShape;
        fixtureDef.density = 1; //TOdo load from tiled

        auto collider = gameObject->addComponent<ColliderComponent>(*gameObject, *rigidComp, fixtureDef);
        collider->registerOnCollisionFunction(
            [](ColliderComponent& self, ColliderComponent& other)
            {
                auto damageComp = other.getGameObject().getComponent<DamageComponent>();
                auto healthComp = self.getGameObject().getComponent<HealthComponent>();

                if (damageComp && healthComp)
                {
                    std::cout << !damageComp->isActive() << std::endl;
                    if (!damageComp->isActive())
                        return;
                    if (damageComp->getOwnerId() != self.getGameObject().getId())
                    {
                        healthComp->takeDamage(damageComp->getDamage());
                    }
                }
            });
    }

    if (spawner)
    {
        auto spawner = gameObject->addComponent<ItemSpawnerComponent>(*gameObject);
        for (auto type : itemType)
        {
            spawner->LoadItem(spriteManager.getWindow(), type);
        }
    }

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}

static GameObject::Ptr loadCollider(tson::Object& object, const std::string& layer)
{
    auto gameObject = GameObject::create(object.getName());
    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));
    bool damage = false;
    for (const auto* property : object.getProperties().get())
    {
        if (property->getName() == "Dead")
            damage = std::any_cast<bool>(property->getValue());
    }

    const auto rigidComp = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_staticBody);

    b2PolygonShape polygonShape{};
    const auto     size = PhysicsManager::t2b(object.getSize());
    polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef{};
    fixtureDef.shape = &polygonShape;

    gameObject->addComponent<ColliderComponent>(*gameObject, *rigidComp, fixtureDef);

    if (damage)
        gameObject->addComponent<DamageComponent>(*gameObject, 9999, gameObject->getId());

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}

static GameObject::Ptr loadTrigger(tson::Object& object, const std::string& layer)
{
    auto gameObject = GameObject::create(object.getName());
    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));
    bool damage = false;
    for (const auto* property : object.getProperties().get())
    {
        if (property->getName() == "Dead")
            damage = std::any_cast<bool>(property->getValue());
    }
    const auto rb = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_staticBody);

    b2PolygonShape polygonShape{};
    const auto     size = PhysicsManager::t2b(object.getSize());
    polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef{};
    fixtureDef.shape    = &polygonShape;
    fixtureDef.isSensor = true;

    if (damage)
        gameObject->addComponent<DamageComponent>(*gameObject, 9999, gameObject->getId());

    auto collider = gameObject->addComponent<ColliderComponent>(*gameObject, *rb, fixtureDef);

    // Add callback to output when something enters the trigger
    collider->registerOnCollisionFunction(
        [](ColliderComponent& self, ColliderComponent& other)
        {
            std::cout << "Entered trigger zone: " << self.getGameObject().getId() << " (collided with "
                      << other.getGameObject().getId() << ")\n";
        });

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}

static GameObject::Ptr loadSpawn(const tson::Object& object, const std::string& layer)
{
    auto gameObject = GameObject::create(object.getName());
    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}
static GameObject::Ptr loadItemSpawner(tson::Object& object, const std::string& layer, const SpriteManager& spriteManager)
{
    auto gameObject = GameObject::create(object.getName());

    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));

    // Parse data from file
    std::vector<ItemType> itemType;

    for (const auto* property : object.getProperties().get())
    {
        if (auto name = property->getName(); name == "Bomb")
        {
            if (std::any_cast<bool>(property->getValue()))
                itemType.push_back(ItemType::Bomb);
        }
        else if (name == "Size")
        {
            if (std::any_cast<bool>(property->getValue()))
                itemType.push_back(ItemType::Size);
        }
    }

    auto spawner = gameObject->addComponent<ItemSpawnerComponent>(*gameObject);
    for (auto type : itemType)
    {
        spawner->LoadItem(spriteManager.getWindow(), type);
    }

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}

GameObject::Ptr ObjectFactory::processTsonObject(tson::Object&        object,
                                                 const tson::Layer&   layer,
                                                 const fs::path&      path,
                                                 const SpriteManager& spriteManager)
{
    // Skip Sprite objects - player/enemy are created via ShipFactory
    if (object.getType() == "Sprite")
    {
        auto sprite = loadSprite(object, layer.getName(), path, spriteManager);
    }
    if (object.getType() == "Collider")
    {
        auto collider = loadCollider(object, layer.getName());
    }
    if (object.getType() == "Trigger")
    {
        auto trigger = loadTrigger(object, layer.getName());
    }
    if (object.getType() == "Spawn")
    {
        auto spawn = loadSpawn(object, layer.getName());
    }
    if (object.getType() == "ItemSpawner")
    {
        auto spawn = loadItemSpawner(object, layer.getName(), spriteManager);
    }

    return {};
}
} // namespace mmt_gd