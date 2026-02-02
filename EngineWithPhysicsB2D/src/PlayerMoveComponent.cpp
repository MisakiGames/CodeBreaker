#include "stdafx.h"

#include "PlayerMoveComponent.hpp"

#include "DeadComponent.h"
#include "GameObject.hpp"
#include "IComponent.hpp"
#include "InputManager.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
PlayerMoveComponent::PlayerMoveComponent(GameObject&         gameObject,
                                         RigidBodyComponent& rigidBody,
                                         DeadComponent&      deadComponent,
                                         int                 playerIndex) :
IComponent(gameObject),
m_playerIndex(playerIndex),
m_rigidBody(rigidBody),
m_deadComponent(deadComponent)
{
}

bool PlayerMoveComponent::init()
{
    m_rigidBody.getB2Body()->SetLinearDamping(2.F);

    return true;
}

void PlayerMoveComponent::update(const float deltaTime)
{
    if (m_deadComponent.isDead())
        return;

    if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
    {
        //translation.x = speed * deltaTime;
        m_rigidBody.setVelocity(sf::Vector2f(300.f, 0.f));
    }
    else if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
    {
        //translation.x = -speed * deltaTime;
        m_rigidBody.setVelocity(sf::Vector2f(-300.f, 0.f));
    }
    else if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
    {
        //translation.y = -speed * deltaTime;
        m_rigidBody.setVelocity(sf::Vector2f(0.f, -300.f));
    }
    else if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
    {
        //translation.y = speed * deltaTime;
        m_rigidBody.setVelocity(sf::Vector2f(0.f, 300.f));
    }
    else
    {
        m_rigidBody.setVelocity(sf::Vector2f(0.f, 0.f));
    }
}
} // namespace mmt_gd