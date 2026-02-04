#include "stdafx.h"

#include "PlayerShootComponent.hpp"

#include "DamageComponent.hpp"
#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "RigidBodyComponent.hpp"

#include <SFML/System.hpp>
#include <iostream>


//static void onBulletCollision(const mmt_gd::BoxColliderComponent& bullet, const mmt_gd::BoxColliderComponent& other)
//{
//    bullet.getGameObject().setActive(false);
//
//    //std::cout << bullet.getGameObject().getId()<< " \tcollision -> \t"<< other.getGameObject().getId()  << std::endl;
//}

mmt_gd::PlayerShootComponent::PlayerShootComponent(
    GameObject&          gameObject,
    const size_t         poolSize,
    sf::RenderWindow&    renderWindow,
    const std::string&   textureFile,
    const sf::IntRect&   m_textureRect,
    const sf::FloatRect& colliderRect,
    const float          bulletMass,
    const std::string&   layerName,
    const bool           useInput) :
IComponent(gameObject),
m_pool(poolSize, textureFile, m_textureRect, layerName, renderWindow, colliderRect, bulletMass /*, onBulletCollision*/),
m_useInput(useInput)
{
}


void mmt_gd::PlayerShootComponent::shoot(sf::Vector2f directionSpeed)
{
    if (m_timeSinceLastShot > 0.2F)
    {
        const auto& bullet = m_pool.get();
        bullet->setActive(true);
        bullet->setPosition(m_gameObject.getPosition() + sf::Vector2f{30.F, 0.F});

        if (auto damageComp = bullet->getComponent<DamageComponent>())
        {
            damageComp->setOwnerId(m_gameObject.getId());
        }

        const auto rb = bullet->getComponent<RigidBodyComponent>();
        // Enable the physics body and set position
        rb->getB2Body()->SetEnabled(true);
        rb->getB2Body()->SetTransform(PhysicsManager::s2b(bullet->getPosition()), 0);
        rb->getB2Body()->SetLinearVelocity(PhysicsManager::s2b(directionSpeed));

        bullet->update(0);
        m_timeSinceLastShot = 0;
    }
}

bool mmt_gd::PlayerShootComponent::init()
{
    return true;
}

void mmt_gd::PlayerShootComponent::update(float deltaTime)
{
    m_timeSinceLastShot += deltaTime;
    if (m_useInput && InputManager::getInstance().isKeyDown("fire", 0))
    {
        shoot(sf::Vector2f{1000, 0});
    }
}
