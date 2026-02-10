//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
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
bool CrownItemComponent::init()
{
    auto sprite          = m_gameObject.getComponent<SpriteRenderComponent>();
    auto spriteSize      = sprite->getSprite().getGlobalBounds().getSize();
    m_centralPointOffset = sf::Vector2f(spriteSize.x / 2, spriteSize.y / 2);
    return true;
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
    const float         radius = 50;
    auto                center = player.getPosition();
    static std::mt19937 gen(std::random_device{}());
    // Distribution for a square from -radius to +radius
    std::uniform_real_distribution<float> dist(-radius, radius);

    float x, y;
    float radiusSquared = radius * radius;

    while (true)
    {
        x = dist(gen);
        y = dist(gen);

        // Check if the point is within the circle's radius
        // We use x*x + y*y instead of sqrt to keep it fast
        if (x * x + y * y <= radiusSquared)
        {
            return sf::Vector2f(center.x + x, center.y + y);
        }
    }
}
void CrownItemComponent::update(float deltaTime)
{
    ItemComponent::update(deltaTime);
    if (m_canBePickedUp && (m_touchingCollision.width > 0 || m_touchingCollision.height > 0))
    {
        setPosition(getClosestPointOutside(m_touchingCollision, getCentralPoint()));
        m_touchingCollision = sf::IntRect(0, 0, 0, 0);
    }
}
sf::Vector2f CrownItemComponent::getCentralPoint()
{
    return m_gameObject.getPosition() + m_centralPointOffset;
}
sf::Vector2f CrownItemComponent::getNearestSpaceSpot(sf::Vector2f point, sf::IntRect rect)
{
    float xMin = rect.left;
    float xMax = rect.left + rect.width;
    float yMin = rect.top;
    float yMax = rect.top + rect.height;
    auto  xPos = std::max<float>(xMin, std::min(point.x, xMax));
    auto  yPos = std::max<float>(yMin, std::min(point.y, yMax));
    return sf::Vector2f(xPos, yPos);
}
sf::Vector2f CrownItemComponent::getClosestPointOutside(const sf::IntRect& rect, sf::Vector2f point)
{
    // Calculate the boundaries
    float left   = static_cast<float>(rect.left);
    float right  = static_cast<float>(rect.left + rect.width);
    float top    = static_cast<float>(rect.top);
    float bottom = static_cast<float>(rect.top + rect.height);

    // 1. Check if the point is outside the rectangle
    if (point.x < left || point.x > right || point.y < top || point.y > bottom)
    {
        // Point is already outside, return it as is
        // (Or clamp it to the edge if you need the closest point ON the edge)
        return point;
    }

    // 2. If we are here, the point is INSIDE.
    // Find the distance to each of the four edges.
    float distToLeft   = point.x - left;
    float distToRight  = right - point.x;
    float distToTop    = point.y - top;
    float distToBottom = bottom - point.y;

    // 3. Find the smallest distance and snap to that edge
    float minDist = std::min({distToLeft, distToRight, distToTop, distToBottom});

    if (minDist == distToLeft)
        point.x = left;
    else if (minDist == distToRight)
        point.x = right;
    else if (minDist == distToTop)
        point.y = top;
    else
        point.y = bottom;

    return point;
}
} // namespace mmt_gd
