//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.hpp"

#include "ObjectFactory.hpp"

#include "ColliderComponent.hpp"
#include "DamageComponent.hpp"
#include "DestructionComponent.hpp"
#include "GameObjectEvents.hpp"
#include "GameObjectManager.hpp"
#include "ItemSpawnerComponent.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "RectComponent.hpp"
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
    sf::IntRect m_textureRect{};
    sf::IntRect m_collisionRect{};
    m_textureRect.width  = object.getSize().x;
    m_textureRect.height = object.getSize().y;
    fs::path              spriteTexture;
    bool                  destroyable = false;
    float                 health      = 0;
    bool                  spawner     = false;
    std::vector<ItemType> itemType;
    std::string           tag = "";

    for (const auto* property : object.getProperties().get())
    {
        if (auto name = property->getName(); name == "Texture")
        {
            spriteTexture = resourcePath / std::any_cast<std::string>(property->getValue());
        }
        else if (name == "TextureRectLeft")
        {
            m_textureRect.left = std::any_cast<int>(property->getValue());
        }
        else if (name == "CollisionTop")
        {
            m_collisionRect.top = std::any_cast<int>(property->getValue());
        }
        else if (name == "CollisionLeft")
        {
            m_collisionRect.left = std::any_cast<int>(property->getValue());
        }
        else if (name == "CollisionWidth")
        {
            m_collisionRect.width = std::any_cast<int>(property->getValue());
        }
        else if (name == "CollisionHeight")
        {
            m_collisionRect.height = std::any_cast<int>(property->getValue());
        }
        else if (name == "TextureRectTop")
        {
            m_textureRect.top = std::any_cast<int>(property->getValue());
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
        else if (property->getName() == "ColliderTag")
            tag = std::any_cast<std::string>(property->getValue());
    }

    if (spriteTexture.string().length() > 0)
    {
        const auto renderComp = gameObject->addComponent<SpriteRenderComponent>(*gameObject,
                                                                                spriteManager.getWindow(),
                                                                                spriteTexture.string(),
                                                                                layer);

        renderComp->getSprite().setTextureRect(m_textureRect);
    }

    const auto rigidComp = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_staticBody);

    if (destroyable)
    {
        auto soundComp = gameObject->addComponent<SoundComponent>(*gameObject);
        soundComp->addSound("damage", "../assets/sounds/impact.wav", 50.0f); // Kiste macht "Pock" bei Treffer
        soundComp->addSound("dying", "../assets/sounds/bomb.wav", 80.0f);    // Kiste explodiert
        std::weak_ptr<SoundComponent> soundWeakPtr = soundComp;
        auto healthComp = gameObject->addComponent<HealthComponent>(*gameObject, health, 1, false);
        std::weak_ptr<HealthComponent> healthWeakPtr = healthComp;
        healthComp->subsribeToOnTakeDamage(
            [soundWeakPtr = soundWeakPtr, healthWeakPtr = healthWeakPtr]()
            {
                if (auto soundComp = soundWeakPtr.lock())
                {
                    if (auto health = healthWeakPtr.lock())
                    {
                        if (; health->isAlive())
                            soundComp->playSound("damage");
                        else
                        {
                            soundComp->playSound("dying");
                        }
                    }
                }
            });
        gameObject->addComponent<DestructionComponent>(*gameObject, *healthComp);

        b2PolygonShape polygonShape;
        if (m_collisionRect.width > 0 && m_collisionRect.height > 0)
        {
            const auto size = PhysicsManager::t2b(tson::Vector2i{m_collisionRect.getSize().x, m_collisionRect.getSize().y},
                                                  true);
            const auto centerPos = PhysicsManager::t2b(tson::Vector2i{m_collisionRect.getSize().x + m_collisionRect.left,
                                                                      m_collisionRect.getSize().y + m_collisionRect.top},
                                                       true);
            const auto centerPosOffset = PhysicsManager::t2b(tson::Vector2i{m_collisionRect.left, m_collisionRect.top}, true);
            polygonShape.SetAsBox(size.x / 2,
                                  size.y / 2,
                                  b2Vec2{centerPos.x / 2 + centerPosOffset.x / 2, centerPos.y / 2 + centerPosOffset.y / 2},
                                  0);
        }
        else
        {
            const auto size = PhysicsManager::t2b(object.getSize(), true);
            polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);
        }
        b2FixtureDef fixtureDef{};
        fixtureDef.shape   = &polygonShape;
        fixtureDef.density = 1; //TOdo load from tiled

        auto collider = gameObject->addComponent<ColliderComponent>(*gameObject, *rigidComp, fixtureDef);
        collider->registerOnCollisionEnterFunction(
            [](ColliderComponent& self, ColliderComponent& other)
            {
                auto damageComp = other.getGameObject().getComponent<DamageComponent>();
                auto healthComp = self.getGameObject().getComponent<HealthComponent>();

                if (damageComp && healthComp)
                {
                    if (!damageComp->isActive())
                        return;
                    if (damageComp->getOwnerId() != self.getGameObject().getId())
                    {
                        healthComp->takeDamage(damageComp->getDamage());
                    }
                }
            });
        collider->registerOnCollisionStayFunction(
            [](ColliderComponent& self, ColliderComponent& other)
            {
                auto damageComp = other.getGameObject().getComponent<DamageComponent>();
                auto healthComp = self.getGameObject().getComponent<HealthComponent>();

                if (damageComp && healthComp)
                {
                    if (!damageComp->isActive())
                        return;
                    if (damageComp->getOwnerId() != self.getGameObject().getId())
                    {
                        healthComp->takeDamage(damageComp->getDamage());
                    }
                }
            });

        if (tag.length() > 0)
            collider->setTag(tag);
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
    bool        damage = false;
    std::string tag    = "";

    for (const auto* property : object.getProperties().get())
    {
        if (property->getName() == "Dead")
            damage = std::any_cast<bool>(property->getValue());
        if (property->getName() == "ColliderTag")
            tag = std::any_cast<std::string>(property->getValue());
    }

    const auto rigidComp = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_staticBody);

    b2PolygonShape polygonShape{};
    const auto     size = PhysicsManager::t2b(object.getSize());
    polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef{};
    fixtureDef.shape = &polygonShape;

    auto collider = gameObject->addComponent<ColliderComponent>(*gameObject, *rigidComp, fixtureDef);

    if (damage)
        gameObject->addComponent<DamageComponent>(*gameObject, 9999, gameObject->getId());

    if (tag.length() > 0)
        collider->setTag(tag);

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
    bool        damage       = false;
    std::string tag          = "";
    float       damageAmount = 0;

    for (const auto* property : object.getProperties().get())
    {
        if (property->getName() == "Dead")
            damage = std::any_cast<bool>(property->getValue());
        if (property->getName() == "ColliderTag")
            tag = std::any_cast<std::string>(property->getValue());
        if (property->getName() == "DamageAmount")
            damageAmount = std::any_cast<float>(property->getValue());
    }
    const auto rb = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_staticBody);

    b2PolygonShape polygonShape{};
    const auto     size = PhysicsManager::t2b(object.getSize());
    polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef{};
    fixtureDef.shape    = &polygonShape;
    fixtureDef.isSensor = true;

    if (damage)
        gameObject->addComponent<DamageComponent>(*gameObject, damageAmount, gameObject->getId());

    auto collider = gameObject->addComponent<ColliderComponent>(*gameObject, *rb, fixtureDef);
    if (tag.length() > 0)
        collider->setTag(tag);

    // Add callback to output when something enters the trigger
#ifdef DEBUG
    collider->registerOnCollisionEnterFunction(
        [](ColliderComponent& self, ColliderComponent& other)
        {
            std::cout << "Entered trigger zone: " << self.getGameObject().getId() << " (collided with "
                      << other.getGameObject().getId() << ")\n";
        });
#endif
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
static GameObject::Ptr loadCrownSpace(const tson::Object& object, const std::string& layer)
{
    auto gameObject = GameObject::create(object.getName());
    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));
    sf::IntRect m_spaceRect;
    m_spaceRect.width  = object.getSize().x;
    m_spaceRect.height = object.getSize().y;

    gameObject->addComponent<RectComponent>(*gameObject, m_spaceRect);

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}
static GameObject::Ptr loadItemSpawner(tson::Object&        object,
                                       const std::string&   layer,
                                       const SpriteManager& spriteManager,
                                       GameObjectManager&   goManager)
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
        spawner->LoadItem(spriteManager.getWindow(), type, goManager);
    }

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}

GameObject::Ptr ObjectFactory::processTsonObject(
    tson::Object&        object,
    const tson::Layer&   layer,
    const fs::path&      path,
    const SpriteManager& spriteManager,
    GameObjectManager&   goManager)
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
        auto itemSpawn = loadItemSpawner(object, layer.getName(), spriteManager, goManager);
    }
    if (object.getType() == "CrownSpace")
    {
        auto crownSpace = loadCrownSpace(object, layer.getName());
    }

    return {};
}
} // namespace mmt_gd