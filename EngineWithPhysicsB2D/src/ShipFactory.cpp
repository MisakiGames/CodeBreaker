#include "stdafx.h"

#include "ShipFactory.hpp"

#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "DamageComponent.hpp"
#include "DestructionComponent.hpp"
#include "EnemyAIComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "HealthComponent.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationSmoothFollow.hpp"

namespace mmt_gd
{
GameObject::Ptr ShipFactory::createPlayerShip(sf::RenderWindow& window)
{
    auto player = GameObject::create("Player");
    player->setPosition(150.0f, 300.0f);

    auto spriteComp = player->addComponent<
        SpriteRenderComponent>(*player, window, "../assets/Hunter1.bmp", "GameObjects", sf::IntRect(49, 25, 23, 23));

    auto rigidBody = player->addComponent<RigidBodyComponent>(*player, b2_dynamicBody);
    rigidBody->getB2Body()->SetFixedRotation(true);

    b2PolygonShape shape;
    const auto     size = PhysicsManager::s2b(sf::Vector2f(32.0f, 32.0f));
    shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape   = &shape;
    fixtureDef.density = 1.0f;

    auto collider = player->addComponent<ColliderComponent>(*player, *rigidBody, fixtureDef);

    collider->registerOnCollisionEnterFunction(
        [](ColliderComponent& self, ColliderComponent& other)
        {
            auto damageComp = other.getGameObject().getComponent<DamageComponent>();
            auto healthComp = self.getGameObject().getComponent<HealthComponent>();

            if (damageComp && healthComp)
            {
                if (damageComp->getOwnerId() != self.getGameObject().getId())
                {
                    healthComp->takeDamage(damageComp->getDamage());
                    other.getGameObject().setActive(false);
                }
            }
        });

    //player->addComponent<PlayerMoveComponent>(*player, *rigidBody, 0);

    player->addComponent<PlayerShootComponent>(*player,
                                               20,
                                               window,
                                               "../assets/ShipGun2.bmp",
                                               sf::IntRect(0, 0, 23, 11),
                                               sf::FloatRect(0, 0, 23, 11),
                                               0.1f,
                                               "GameObjects");

    auto health = player->addComponent<HealthComponent>(*player, 100, true);

    if (!player->init())
    {
        sf::err() << "Could not initialize player ship\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(player));

    return player;
}

GameObject::Ptr ShipFactory::createEnemyShip(sf::RenderWindow& window, GameObject* playerTarget)
{
    auto enemy = GameObject::create("Enemy");
    enemy->setPosition(700.0f, 300.0f);

    auto spriteComp = enemy->addComponent<
        SpriteRenderComponent>(*enemy, window, "../assets/Hunter2.bmp", "GameObjects", sf::IntRect(49, 25, 23, 23));

    auto rigidBody = enemy->addComponent<RigidBodyComponent>(*enemy, b2_dynamicBody);
    rigidBody->getB2Body()->SetFixedRotation(true);

    b2PolygonShape shape;
    const auto     size = PhysicsManager::s2b(sf::Vector2f(32.0f, 32.0f));
    shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape   = &shape;
    fixtureDef.density = 1.0f;

    auto collider = enemy->addComponent<ColliderComponent>(*enemy, *rigidBody, fixtureDef);

    collider->registerOnCollisionEnterFunction(
        [](ColliderComponent& self, ColliderComponent& other)
        {
            auto damageComp = other.getGameObject().getComponent<DamageComponent>();
            auto healthComp = self.getGameObject().getComponent<HealthComponent>();

            if (damageComp && healthComp)
            {
                if (damageComp->getOwnerId() != self.getGameObject().getId())
                {
                    healthComp->takeDamage(damageComp->getDamage());
                    other.getGameObject().setActive(false);
                }
            }
        });

    auto shootComp = enemy->addComponent<PlayerShootComponent>(*enemy,
                                                               10,
                                                               window,
                                                               "../assets/ShipGun2.bmp",
                                                               sf::IntRect(0, 0, 23, 11),
                                                               sf::FloatRect(0, 0, 23, 11),
                                                               0.1f,
                                                               "GameObjects",
                                                               false); // false = don't use player input

    enemy->addComponent<EnemyAIComponent>(*enemy, *rigidBody, *shootComp, playerTarget, 100.0f, 1.5f, 100.0f, 500.0f);

    /*auto health = enemy->addComponent<HealthComponent>(*enemy, 3, false);

    enemy->addComponent<DestructionComponent>(*enemy, *health);*/

    if (!enemy->init())
    {
        sf::err() << "Could not initialize enemy ship\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(enemy));

    return enemy;
}
} // namespace mmt_gd