#include "stdafx.h"

#include "PlayerFactory.h"

#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "DamageComponent.hpp"
#include "DashReferenceComponent.h"
#include "DeadComponent.h"
#include "DestructionComponent.hpp"
#include "EnemyAIComponent.hpp"
#include "EventBus.hpp"
#include "FollowPlayer.h"
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

#include <sstream>

namespace mmt_gd
{
GameObject::Ptr PlayerFactory::createPlayer(
    sf::RenderWindow&  window,
    enum PlayerSpawn   spawnName,
    GameObjectManager& goManager,
    int                plrIndex,
    std::string        color)
{
    const float scaleFaktor = 1;
    std::string spawn       = "";
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
    auto player     = GameObject::create("Player_" + color);
    player->setPosition(spawnPoint);

    auto                          soundComponent = player->addComponent<SoundComponent>(*player);
    std::weak_ptr<SoundComponent> soundWeakPtr   = soundComponent;

    soundComponent->addSound("dash", "../assets/sounds/dash.wav", 70.0f);
    soundComponent->addSound("step", "../assets/sounds/step.wav", 100.0f);
    soundComponent->addSound("impact", "../assets/sounds/impact.wav", 100.0f);

    soundComponent->addSound("damage", "../assets/sounds/damage.wav", 90.0f);
    soundComponent->addSound("item", "../assets/sounds/item.wav", 70.0f);
    soundComponent->addSound("crown_drop", "../assets/sounds/crown_drop.flac", 85.0f);

    soundComponent->addSound("dying", "../assets/sounds/dying.wav", 95.0f);

    player->setScale(scaleFaktor, scaleFaktor);
    auto playerRect = sf::IntRect(15, 18, 18, 26);
    //auto playerRect = sf::IntRect(18, 20, 12, 24);
    auto spriteComp = player->addComponent<
        SpriteAnimationRenderComponent>(*player, window, "GameObjects", playerRect, sf::Vector2f(8, 6));
    std::weak_ptr<SpriteAnimationRenderComponent> spriteWeakPtr = spriteComp;
    std::stringstream                             playerSpriteStream;
    playerSpriteStream << "../assets/Character/" << color << "/";

    spriteComp->loadAndMapTexture(playerSpriteStream.str() + "Walk.png", AnimationState::Walk, 5);
    spriteComp->loadAndMapTexture(playerSpriteStream.str() + "Idle.png", AnimationState::Idle, 5);
    spriteComp->loadAndMapTexture(playerSpriteStream.str() + "Dash.png", AnimationState::Dash, 10, false);
    spriteComp->loadAndMapTexture(playerSpriteStream.str() + "Death.png", AnimationState::Dead, 10, false);

    auto                           health        = player->addComponent<HealthComponent>(*player, 100, 1, false);
    std::weak_ptr<HealthComponent> healthWeakPtr = health;
    health->subsribeToOnTakeDamage(
        [soundWeakPtr = soundWeakPtr, healthWeakPtr = healthWeakPtr]()
        {
            if (auto soundComp = soundWeakPtr.lock())
                if (auto healthComp = healthWeakPtr.lock(); healthComp->isAlive())
                    soundComp->playSound("damage");
                else
                    soundComp->playSound("dying");
        });

    auto rigidBody  = player->addComponent<RigidBodyComponent>(*player, b2_dynamicBody);
    auto damageComp = player->addComponent<DamageComponent>(*player, 10, player->getId());
    damageComp->setActive(false);
    std::weak_ptr<DamageComponent> damageWeakPtr = damageComp;
    auto                           respawn       = player->addComponent<RespawnComponent>(*player);

    auto deadComp = player->addComponent<DeadComponent>(*player, *health, *respawn, 2);
    deadComp->subscribeToDeath(
        [spriteWeakPtr = spriteWeakPtr]()
        {
            if (auto spriteComp = spriteWeakPtr.lock())
                spriteComp->setState(AnimationState::Dead);
        });
    std::weak_ptr<DeadComponent> deadWeakComp = deadComp;
    respawn->SubscribeToOnRespawn(
        [deadWeakComp = deadWeakComp]()
        {
            if (auto deadComp = deadWeakComp.lock())
                deadComp->setAlive();
        });
    respawn->SubscribeToOnRespawn(
        [healthWeakPtr = healthWeakPtr]()
        {
            if (auto healthComp = healthWeakPtr.lock())
                healthComp->fullHealth();
        });
    respawn->SubscribeToOnRespawn(
        [healthWeakPtr = healthWeakPtr]()
        {
            if (auto healthComp = healthWeakPtr.lock())
                healthComp->setInvincible(false);
        });
    rigidBody->getB2Body()->SetFixedRotation(true);

    b2PolygonShape shape;
    const auto size = PhysicsManager::s2b(sf::Vector2f(playerRect.width * scaleFaktor, playerRect.height / 2 * scaleFaktor));
    shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y * 1.5f}, 0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape   = &shape;
    fixtureDef.density = 1.0f;
    auto move = player->addComponent<PlayerMoveComponent>(*player, *rigidBody, *deadComp, *damageComp, plrIndex);

    move->subscribeToOnWhileDash(
        [spriteWeakPtr = spriteWeakPtr]()
        {
            if (auto spriteComp = spriteWeakPtr.lock())
                spriteComp->setState(AnimationState::Dash);
        });
    move->subscribeToOnDash(
        [damageWeakPtr = damageWeakPtr]()
        {
            if (auto damageComp = damageWeakPtr.lock())
                damageComp->setActive(true);
        });
    move->subscribeToOnDashEnd(
        [damageWeakPtr = damageWeakPtr]()
        {
            if (auto damageComp = damageWeakPtr.lock())
                damageComp->setActive(false);
        });
    move->subscribeToOnDash(
        [soundWeakPtr = soundWeakPtr]()
        {
            if (auto soundComp = soundWeakPtr.lock())
                soundComp->playSound("dash");
        });
    move->subscribeToOnMoved(
        [soundWeakPtr = soundWeakPtr]()
        {
            if (auto soundComp = soundWeakPtr.lock())
                soundComp->playSound("step", true);
        });

    auto                                score        = player->addComponent<PlayerScoreComponent>(*player);
    std::weak_ptr<PlayerScoreComponent> scoreWeakPtr = score;
    respawn->SubscribeToOnRespawn(
        [scoreWeakPtr = scoreWeakPtr]()
        {
            if (auto scoreComp = scoreWeakPtr.lock())
                scoreComp->removePoints(10);
        });

    auto                           pickup        = player->addComponent<PickupComponent>(*player, *score);
    std::weak_ptr<PickupComponent> pickupWeakPtr = pickup;

    pickup->subscribeToOnPickup(
        [soundWeakPtr = soundWeakPtr]()
        {
            if (auto soundComp = soundWeakPtr.lock())
                soundComp->playSound("item");
        });
    pickup->subscribeToOnLoseCrown(
        [soundWeakPtr = soundWeakPtr]()
        {
            if (auto soundComp = soundWeakPtr.lock())
                soundComp->playSound("crown_drop");
        });

    deadComp->subscribeToDeath(
        [pickupWeakPtr = pickupWeakPtr]()
        {
            if (auto pickupComp = pickupWeakPtr.lock())
                pickupComp->loseAllItem();
        });
    health->subsribeToOnTakeDamage(
        [pickupWeakPtr = pickupWeakPtr]()
        {
            if (auto pickupComp = pickupWeakPtr.lock())
                pickupComp->loseCrown();
        });

    auto collider = player->addComponent<ColliderComponent>(*player, *rigidBody, fixtureDef);
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
                    if (other.getTag() == "InstaKill")
                    {
                        healthComp->kill();
                    }
                    else if (other.getTag() == "DamageOverTime")
                    {
                        healthComp->setDamagePerSecond(damageComp->getDamage());
                    }
                    else if (other.getTag() != self.getGameObject().getId())
                    {
                        healthComp->takeDamage(damageComp->getDamage());
                    }
                }
            }
        });
    collider->registerOnCollisionEnterFunction(
        [soundWeakPtr = soundWeakPtr](ColliderComponent& self, ColliderComponent& other)
        {
            if (other.isSensor())
                return;
            if (auto soundComp = soundWeakPtr.lock())
                soundComp->playSound("impact");
        });
    collider->registerOnCollisionExitFunction(
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
                    if (other.getTag() == "DamageOverTime")
                        healthComp->setDamagePerSecond(0);
                }
            }
        });
    collider->registerOnCollisionEnterFunction(
        [](ColliderComponent& self, ColliderComponent& other)
        {
            auto moveComp = self.getGameObject().getComponent<PlayerMoveComponent>();

            if (moveComp)
            {
                moveComp->OnCollision();
            }
        });
    collider->registerOnCollisionEnterFunction(
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

    auto dashGO = GameObject::create("Dash_" + color);
    dashGO->setPosition(player->getPosition());
    auto dashRigidBody = dashGO->addComponent<RigidBodyComponent>(*dashGO, b2_staticBody);
    dashGO->addComponent<FollowPlayer>(*dashGO, *player);
    b2PolygonShape dashShape;
    const auto dashSize = PhysicsManager::s2b(sf::Vector2f(playerRect.width * scaleFaktor, playerRect.height * scaleFaktor));
    dashShape.SetAsBox(dashSize.x / 2, dashSize.y / 2, b2Vec2{dashSize.x / 2, dashSize.y / 2}, 0);

    b2FixtureDef dashFixtureDef;
    dashFixtureDef.shape    = &dashShape;
    dashFixtureDef.density  = 1.0f;
    dashFixtureDef.isSensor = true;

    auto dashCollider = dashGO->addComponent<ColliderComponent>(*dashGO, *dashRigidBody, dashFixtureDef);

    auto dashDamageComp = dashGO->addComponent<DamageComponent>(*dashGO, 15, player->getId());
    dashDamageComp->setActive(false);
    std::weak_ptr<DamageComponent> dashDamageWeakPtr = dashDamageComp;

    move->subscribeToOnDash(
        [damageWeakPtr = dashDamageWeakPtr]()
        {
            if (auto damageComp = damageWeakPtr.lock())
                damageComp->setActive(true);
        });
    move->subscribeToOnDashEnd(
        [damageWeakPtr = dashDamageWeakPtr]()
        {
            if (auto damageComp = damageWeakPtr.lock())
                damageComp->setActive(false);
        });

    player->addComponent<DashReferenceComponent>(*player, *dashGO);

    if (!player->init())
    {
        sf::err() << "Could not initialize player\n";
    }
    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(player));

    if (!dashGO->init())
    {
        sf::err() << "Could not initialize player\n";
    }
    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(dashGO));

    return player;
}
} // namespace mmt_gd