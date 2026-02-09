#include "stdafx.h"

#include "BombItemComponent.h"

#include "BombAnimationComponent.h"
#include "ColliderComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteAnimationRenderComponent.h"
#include "SpriteRenderComponent.hpp"

namespace mmt_gd
{
void BombItemComponent::use(GameObject& player)
{
    m_player      = &player;
    auto collider = m_bomb->getComponent<ColliderComponent>();
    collider->setTag(m_player->getId());
}
void BombItemComponent::stopUse(GameObject& player)
{
    m_spawnTime   = 0;
    m_player      = nullptr;
    auto collider = m_bomb->getComponent<ColliderComponent>();
    collider->setTag("");
}
void BombItemComponent::update(float deltaTime)
{
    ItemComponent::update(deltaTime);
    if (!m_player)
        return;
    m_spawnTime += deltaTime;
    if (m_spawnTime >= m_maxSpawnTime)
    {
        explode();
    }
}
void BombItemComponent::explode()
{
    auto spriteAnimComp = m_player->getComponent<SpriteAnimationRenderComponent>();
    auto rectPlayer     = spriteAnimComp->getSprite().getTextureRect();
    auto scale          = m_player->getScale();
    auto bombScale      = m_bomb->getScale();
    auto rectBomb       = m_bomb->getComponent<BombAnimationComponent>()->getSprite().getTextureRect();

    const float y_offset = ((rectPlayer.getSize().y * scale.y) / 2) - (rectBomb.getSize().y * bombScale.y) / 2;
    const float x_offset = ((rectPlayer.getSize().x * scale.x) / 2) - (rectBomb.getSize().x * bombScale.y) / 2;
    auto        pos      = m_player->getPosition();
    pos                  = sf::Vector2f(pos.x + x_offset, pos.y + y_offset);
    std::cout << pos.x << "," << pos.y << std::endl;
    auto bombRigid = m_bomb->getComponent<RigidBodyComponent>();
    bombRigid->getB2Body()->SetTransform(PhysicsManager::s2b(pos), 0);
    auto bombAnimation = m_bomb->getComponent<BombAnimationComponent>();
    bombAnimation->resetAnimation();
    m_spawnTime   = 0;
    auto collider = m_bomb->getComponent<ColliderComponent>();
    collider->getFixture()->Refilter();
    for (auto sub : m_onExplode)
        sub();
}
} // namespace mmt_gd