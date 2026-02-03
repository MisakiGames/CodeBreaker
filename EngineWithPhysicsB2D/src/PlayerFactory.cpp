#include "stdafx.h"

#include "PlayerFactory.h"

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
#include "PickupComponent.h"
#include "PlayerMoveComponent.hpp"
#include "PlayerScoreComponent.h"
#include "PlayerShootComponent.hpp"
#include "RespawnComponent.h"
#include "RigidBodyComponent.hpp"
#include "SpriteAnimationRenderComponent.h"
#include "SpriteRenderComponent.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationSmoothFollow.hpp"

namespace mmt_gd
{
GameObject::Ptr PlayerFactory::createPlayer(sf::RenderWindow&  window,
                                            enum PlayerSpawn   spawnName,
                                            GameObjectManager& goManager,
                                            int                plrIndex)
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

    auto spriteComp = player->addComponent<
        SpriteAnimationRenderComponent>(*player, window, "GameObjects", sf::IntRect(18, 20, 12, 24), sf::Vector2f(8, 6));

    spriteComp->loadAndMapTexture("../assets/Character/Red/Walk.png", AnimationState::Walk, 5);
    spriteComp->loadAndMapTexture("../assets/Character/Red/Idle.png", AnimationState::Idle, 5);
    spriteComp->loadAndMapTexture("../assets/Character/Red/Dash.png", AnimationState::Dash, 10, false);
    spriteComp->loadAndMapTexture("../assets/Character/Red/Death.png", AnimationState::Dead, 10, false);
    auto health     = player->addComponent<HealthComponent>(*player, 100, true);
    auto rigidBody  = player->addComponent<RigidBodyComponent>(*player, b2_dynamicBody);
    auto damageComp = player->addComponent<DamageComponent>(*player, 10, player->getId());
    damageComp->setActive(false);
    auto respawn = player->addComponent<RespawnComponent>(*player);

    auto deadComp = player->addComponent<DeadComponent>(*player, *health, *respawn, 2);
    deadComp->subscribeToDeath([spriteComp = spriteComp]() { spriteComp->setState(AnimationState::Dead); });

    respawn->SubscribeToOnRespawn([deadComp = deadComp]() { deadComp->setAlive(); });
    respawn->SubscribeToOnRespawn([healthComp = health]() { healthComp->fullHealth(); });
    respawn->SubscribeToOnRespawn([healthComp = health]() { healthComp->setInvincible(false); });
    rigidBody->getB2Body()->SetFixedRotation(true);

    b2PolygonShape shape;
    const auto     size = PhysicsManager::s2b(sf::Vector2f(12.0f, 24.0f));
    shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y / 2}, 0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape   = &shape;
    fixtureDef.density = 1.0f;
    auto move          = player->addComponent<PlayerMoveComponent>(*player, *rigidBody, *deadComp, plrIndex);
    move->subscribeToOnDash([spriteComp = spriteComp]() { spriteComp->setState(AnimationState::Dash); });
    move->subscribeToOnDash(
        [damageComp = damageComp]()
        {
            damageComp->setActive(true);
        });
    move->subscribeToOnDashEnd(
        [damageComp = damageComp]()
        {
            damageComp->setActive(false);
        });

    auto score = player->addComponent<PlayerScoreComponent>(*player);
    respawn->SubscribeToOnRespawn([scoreComp = score]() { scoreComp->removePoints(10); });

    auto pickup = player->addComponent<PickupComponent>(*player, *score);
    deadComp->subscribeToDeath([pickupComp = pickup]() { pickupComp->loseItem(); });

    auto collider = player->addComponent<ColliderComponent>(*player, *rigidBody, fixtureDef);
    collider->registerOnCollisionFunction(
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
                    healthComp->setInvincible(true);
                }
            }
        });
    collider->registerOnCollisionFunction(
        [](ColliderComponent& self, ColliderComponent& other)
        {
            auto moveComp = self.getGameObject().getComponent<PlayerMoveComponent>();

            if (moveComp)
            {
                moveComp->OnCollision();
            }
        });
    collider->registerOnCollisionFunction(
        [](ColliderComponent& self, ColliderComponent& other)
        {
            auto itemComp   = other.getGameObject().getComponent<ItemComponent>();
            auto pickupComp = self.getGameObject().getComponent<PickupComponent>();
            if (itemComp && pickupComp)
            {
                if (itemComp->canBePickedUp())
                    pickupComp->pickup(*itemComp);
            }
        });

    if (!player->init())
    {
        sf::err() << "Could not initialize player ship\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(player));

    return player;
}
} // namespace mmt_gd