//comment
#include "stdafx.h"

#include "ResizeItemComponent.h"

#include "ColliderComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "SpriteAnimationRenderComponent.h"

namespace mmt_gd
{
void ResizeItemComponent::use(GameObject& player)
{
    m_player   = &player;
    m_oldScale = m_player->getScale().x;
    std::cout << m_oldScale << std::endl;
    auto sprite = player.getComponent<SpriteAnimationRenderComponent>();
    m_big    = true;
    m_resize = true;
}
void ResizeItemComponent::stopUse(GameObject& player)
{
    auto sprite = player.getComponent<SpriteAnimationRenderComponent>();
    m_big    = false;
    m_resize = true;
}
void ResizeItemComponent::update(float deltaTime)
{
    ItemComponent::update(deltaTime);
    if (m_player && m_resize)
    {
        float scale = m_oldScale;
        if (m_big)
        {
            scale = m_newScale;
        }
        auto     sprite      = m_player->getComponent<SpriteAnimationRenderComponent>();
    sprite->setScale(scale);
        auto     collider    = m_player->getComponent<ColliderComponent>();
        auto     fixture     = collider->getFixture();
        auto     userData    = fixture->GetUserData();
        b2Filter filter      = fixture->GetFilterData();
        float    friction    = fixture->GetFriction();
        float    restitution = fixture->GetRestitution();
        auto     player_size = sprite->getSprite().getGlobalBounds().getSize();
        ;
        b2PolygonShape shape;
        auto           newSize = sf::Vector2f(player_size.x * scale, player_size.y * scale / 2);
        const auto     size    = PhysicsManager::s2b(newSize);
        shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{size.x / 2, size.y * 1.5f}, 0);

        b2FixtureDef fixtureDef;
        fixtureDef.shape   = &shape;
        fixtureDef.density = 1.0f;

        fixtureDef.filter      = filter;
        fixtureDef.friction    = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.userData    = userData;

        // 4. Swap fixtures
        collider->getBody().getB2Body()->DestroyFixture(fixture);
        fixture = collider->getBody().getB2Body()->CreateFixture(&fixtureDef);

        // 5. Recalculate mass for the new size
        collider->getBody().getB2Body()->ResetMassData();
        m_resize = false;
        if (!m_big)
            m_player = nullptr;
    }
}
} // namespace mmt_gd
