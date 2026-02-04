#include "stdafx.h"

#include "CrownItemComponent.h"

#include "GameObject.hpp"
#include "PlayerScoreComponent.h"

namespace mmt_gd
{
void CrownItemComponent::use(GameObject& player)
{
    const float y_offset = -15;
    const float x_offset = -2.5f;
    auto        pos      = player.getPosition();
    pos                  = sf::Vector2f(pos.x + x_offset, pos.y + y_offset);
    setPosition(pos);
    auto scoreComp = player.getComponent<PlayerScoreComponent>();

    scoreComp->setHasCrown(true);
}
void CrownItemComponent::stopUse(GameObject& player)
{
    auto scoreComp = player.getComponent<PlayerScoreComponent>();

    scoreComp->setHasCrown(false);
}
} // namespace mmt_gd