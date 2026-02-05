#include "stdafx.h"

#include "BulletPool.hpp"

#include "ColliderComponent.hpp"
#include "DamageComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "PhysicsManager.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"

#include <iostream>
#include <memory>

mmt_gd::BulletPool::BulletPool(
    const size_t size,
    const std::string& textureFile,
    sf::IntRect m_textureRect,
    const std::string& layerName,
    sf::RenderWindow& renderWindow,
    const sf::FloatRect& colliderRect,
    float mass/*,
    std::function<void(BoxColliderComponent& a, BoxColliderComponent& b)> collisionCallback*/) :
    m_pool(size, nullptr) /*,
m_collisionCallback(std::move(collisionCallback))*/
{
    for (auto& gameObject : m_pool)
    {
        gameObject = std::make_shared<GameObject>("Bullet_" + std::to_string(m_globalBulletIdx++));
        gameObject->setPosition(-1000, -1000);

        const auto renderComp = gameObject->addComponent<
            SpriteRenderComponent>(*gameObject, renderWindow, textureFile, layerName, m_textureRect);

        auto rigidBodyComponent = gameObject->addComponent<RigidBodyComponent>(*gameObject, b2_dynamicBody);
        rigidBodyComponent->getB2Body()->SetFixedRotation(true);

        auto fixtureDef     = b2FixtureDef{};
        fixtureDef.density  = 0.1F;
        fixtureDef.isSensor = true; // Bullets are sensors - detect collision but don't push
        auto shape          = b2PolygonShape{};

        // Convert colliderRect from pixels to Box2D units (colliderRect is in pixels)
        const float halfWidth  = colliderRect.width * 0.5f * PhysicsManager::UNRATIO;
        const float halfHeight = colliderRect.height * 0.5f * PhysicsManager::UNRATIO;
        // Offset center to align with sprite (drawn from top-left)
        shape.SetAsBox(halfWidth, halfHeight, b2Vec2{halfWidth, halfHeight}, 0);
        fixtureDef.shape = &shape;

        auto boxColliderComponent = gameObject->addComponent<ColliderComponent>(*gameObject, *rigidBodyComponent, fixtureDef);

        // Register collision callback to deactivate bullet on impact
        boxColliderComponent->registerOnCollisionEnterFunction(
            [](ColliderComponent& self, ColliderComponent& other)
            {
                auto damageComp = self.getGameObject().getComponent<DamageComponent>();
                if (damageComp)
                {
                    // Don't collide with owner
                    if (damageComp->getOwnerId() == other.getGameObject().getId())
                    {
                        return;
                    }
                }
                // Mark bullet for deactivation (don't modify physics body during callback)
                self.getGameObject().setActive(false);
            });

        gameObject->addComponent<DamageComponent>(*gameObject, 1, "");

        if (!gameObject->init())
        {
            sf::err() << "Could not initialize go " << gameObject->getId() << '\n';
        }

        rigidBodyComponent->getB2Body()->SetEnabled(false);

        gameObject->setActive(false);
        EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));
    }

    std::cout << "## pool-size: " << m_pool.size() << '\n';
}

mmt_gd::GameObject::Ptr mmt_gd::BulletPool::get()
{
    return m_pool[m_counter++ % m_pool.size()];
}