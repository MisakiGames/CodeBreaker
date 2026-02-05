#include "stdafx.h"

#include "CrownItemComponent.h"

#include "GameObject.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerScoreComponent.h"
#include "SpriteAnimationRenderComponent.h"
#include "SpriteRenderComponent.hpp"

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
}
} // namespace mmt_gd