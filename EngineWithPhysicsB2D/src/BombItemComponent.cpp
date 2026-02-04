#include "stdafx.h"

#include "BombItemComponent.h"

namespace mmt_gd
{
void BombItemComponent::use(GameObject& player)
{
    m_player = &player;
}
void BombItemComponent::stopUse(GameObject& player)
{
    m_player = nullptr;
}
} // namespace mmt_gd