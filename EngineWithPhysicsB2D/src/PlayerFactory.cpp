#include "stdafx.h"

#include "PlayerFactory.h"

#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "DamageComponent.hpp"
#include "DestructionComponent.hpp"
#include "EnemyAIComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "GameObjectManager.hpp"
#include "HealthComponent.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteAnimationRenderComponent.h"
#include "SpriteRenderComponent.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationSmoothFollow.hpp"

namespace mmt_gd
{
GameObject::Ptr PlayerFactory::createPlayer(sf::RenderWindow& window, enum PlayerSpawn spawnName, GameObjectManager& goManager)
{
    std::string spawn = "";
    switch (spawnName)
    {
        case mmt_gd::PlayerSpawn::TopLeft:
            spawn = "TopLeftSpawn";
            break;
        case mmt_gd::PlayerSpawn::BottomLeft:
            spawn = "BottomLeftSpawn";
            break;
        case mmt_gd::PlayerSpawn::TopRight:
            spawn = "TopRightSpawn";
            break;
        case mmt_gd::PlayerSpawn::BottomRight:
            spawn = "BottomRightSpawn";
            break;
        default:
            break;
    }

    auto spawnPoint = goManager.getGameObject(spawn)->getPosition();
    auto player     = GameObject::create("Player");
    player->setPosition(spawnPoint);

    auto spriteComp = player->addComponent<SpriteAnimationRenderComponent>(*player,
                                                                           window,
                                                                           "../assets/Character/Red/Walk.png",
                                                                           "GameObjects",
                                                                           sf::IntRect(18, 20, 12, 24),
                                                                           sf::Vector2f(8, 6));

    auto rigidBody = player->addComponent<RigidBodyComponent>(*player, b2_dynamicBody);
    rigidBody->getB2Body()->SetFixedRotation(true);

    b2PolygonShape shape;
    const auto     size = PhysicsManager::s2b(sf::Vector2f(32.0f, 32.0f));
    shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape   = &shape;
    fixtureDef.density = 1.0f;
    player->addComponent<PlayerMoveComponent>(*player, *rigidBody, 0);
    auto collider = player->addComponent<ColliderComponent>(*player, *rigidBody, fixtureDef);
    auto health   = player->addComponent<HealthComponent>(*player, 100, true);

    if (!player->init())
    {
        sf::err() << "Could not initialize player ship\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(player));

    return player;
}
} // namespace mmt_gd