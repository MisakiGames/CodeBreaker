#include "stdafx.h"
#include "ColliderComponent.hpp"
#include "GameObjectEvents.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "Tileson.hpp"
#include <iostream>

#include "ObjectFactory.hpp"


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
    fs::path spriteTexture;
    fs::path bulletTexture;
    auto     input     = false;
    auto     playerIdx = 0;

    for (const auto* property : object.getProperties().get())
    {
        if (auto name = property->getName(); name == "Texture")
        {
            spriteTexture = resourcePath / std::any_cast<std::string>(property->getValue());
        }
        else if (name == "BulletTexture")
        {
            bulletTexture = resourcePath / std::any_cast<std::string>(property->getValue());
        }
        else if (name == "TextureRectLeft")
        {
            textureRect.left = std::any_cast<int>(property->getValue());
        }
        else if (name == "TextureRectTop")
        {
            textureRect.top = std::any_cast<int>(property->getValue());
        }
        else if (name == "InputPlayerIdx")
        {
            input     = true;
            playerIdx = std::any_cast<int>(property->getValue());
        }

        else if (name == "Mass")
        {
            auto mass = std::any_cast<float>(property->getValue());
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

    if (bulletTexture.string().length() > 0)
    {
        gameObject->addComponent<PlayerShootComponent>(*gameObject,
                                                       5,
                                                       spriteManager.getWindow(),
                                                       bulletTexture.string(),
                                                       textureRect,
                                                       sf::FloatRect{0, 0, 49, 22},
                                                       0.1F,
                                                       layer);
    }

    const auto rigidComp = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_dynamicBody);

    b2PolygonShape polygonShape;
    const auto     size = PhysicsManager::t2b(object.getSize(), true);
    polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);
    b2FixtureDef fixtureDef{};
    fixtureDef.shape   = &polygonShape;
    fixtureDef.density = 1; //TOdo load from tiled


    gameObject->addComponent<ColliderComponent>(*gameObject, *rigidComp, fixtureDef);


    if (input)
    {
        gameObject->addComponent<PlayerMoveComponent>(*gameObject, *rigidComp, playerIdx);
    }

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}

static GameObject::Ptr loadCollider(const tson::Object& object, const std::string& layer)
{
    auto gameObject = GameObject::create(object.getName());
    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));

    const auto rigidComp = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_staticBody);

    b2PolygonShape polygonShape{};
    const auto     size = PhysicsManager::t2b(object.getSize());
    polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef{};
    fixtureDef.shape = &polygonShape;

    gameObject->addComponent<ColliderComponent>(*gameObject, *rigidComp, fixtureDef);

    if (!gameObject->init())
    {
        sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

    return gameObject;
}

static GameObject::Ptr loadTrigger(const tson::Object& object, const std::string& layer)
{
    auto gameObject = GameObject::create(object.getName());
    gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));

    const auto rb = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_staticBody);

    b2PolygonShape polygonShape{};
    const auto     size = PhysicsManager::t2b(object.getSize());
    polygonShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);


    b2FixtureDef fixtureDef{};
    fixtureDef.shape    = &polygonShape;
    fixtureDef.isSensor = true;

    auto collider = gameObject->addComponent<ColliderComponent>(*gameObject, *rb, fixtureDef);
    
    // Add callback to output when something enters the trigger
    collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {
        std::cout << "Entered trigger zone: " << self.getGameObject().getId() 
                  << " (collided with " << other.getGameObject().getId() << ")\n";
    });

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
    // if (object.getType() == "Sprite")
    // {
    //     auto sprite = loadSprite(object, layer.getName(), path, spriteManager);
    // }
    if (object.getType() == "Collider")
    {
        auto collider = loadCollider(object, layer.getName());
    }
    if (object.getType() == "Trigger")
    {
        auto trigger = loadTrigger(object, layer.getName());
    }

    return {};
}
} // namespace mmt_gd
