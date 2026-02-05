#include "stdafx.h"

#include "CrownItemComponent.h"

#include "GameObject.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerScoreComponent.h"
#include "RectComponent.h"
#include "SpriteAnimationRenderComponent.h"
#include "SpriteRenderComponent.hpp"
#include <random>

namespace mmt_gd
{
void CrownItemComponent::use(GameObject& player)
{
    auto spriteAnimComp = player.getComponent<SpriteAnimationRenderComponent>();
    auto rectPlayer     = spriteAnimComp->getSprite().getTextureRect();
    auto scale          = player.getScale();
    auto rectCrown      = m_gameObject.getComponent<SpriteRenderComponent>()->getSprite().getTextureRect();

    const float y_offset = -15;
    const float x_offset = ((rectPlayer.getSize().x * scale.x) / 2) - rectCrown.getSize().x / 2;
    auto        pos      = player.getPosition();
    pos                  = sf::Vector2f(pos.x + x_offset, pos.y + y_offset);
    setPosition(pos);
    player.getComponent<PlayerMoveComponent>()->deactivateDash();

    auto scoreComp = player.getComponent<PlayerScoreComponent>();

    scoreComp->setHasCrown(true);
}
void CrownItemComponent::stopUse(GameObject& player)
{
    player.getComponent<PlayerMoveComponent>()->activateDash();
    auto scoreComp = player.getComponent<PlayerScoreComponent>();

    scoreComp->setHasCrown(false);
    auto rect = m_crownSpace.getComponent<RectComponent>()->m_rect;
    setPosition(getNearestSpaceSpot(getRandomPointInCircle(player), rect));
}
sf::Vector2f CrownItemComponent::getRandomPointInCircle(GameObject& player)
{
    const float radius = 50;
    auto        center = player.getPosition();
    // Setup random number engine (C++11)
    static std::mt19937                   gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float angle = dist(gen) * 2.0f * 3.14159f;   // Random angle
    float r     = std::sqrt(dist(gen)) * radius; // Square root for uniform distribution

    return sf::Vector2f(center.x + r * std::cos(angle), center.y + r * std::sin(angle));
}
void CrownItemComponent::update(float deltaTime)
{
    ItemComponent::update(deltaTime);
}
sf::Vector2f CrownItemComponent::getNearestSpaceSpot(sf::Vector2f point, sf::IntRect rect)
{
    float xMin = rect.left;
    float xMax = rect.left + rect.width;
    float yMin = rect.top;
    float yMax = rect.top + rect.height;
    auto xPos = std::max<float>(xMin, std::min(point.x, xMax));
    auto yPos = std::max<float>(yMin, std::min(point.y, yMax));
    return sf::Vector2f(xPos, yPos);

}
} // namespace mmt_gd