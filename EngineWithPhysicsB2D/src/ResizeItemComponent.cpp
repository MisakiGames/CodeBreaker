#include "stdafx.h"

#include "ResizeItemComponent.h"

#include "ColliderComponent.hpp"
#include "GameObject.hpp"
#include "SpriteAnimationRenderComponent.h"

namespace mmt_gd
{
void ResizeItemComponent::use(GameObject& player)
{
    auto sprite = player.getComponent<SpriteAnimationRenderComponent>();
    sprite->setScale(scaleFactor);
    auto collider = player.getComponent<ColliderComponent>();
    collider->setScale(sf::Vector2(scaleFactor, scaleFactor));
}
void ResizeItemComponent::stopUse(GameObject& player)
{
    auto sprite = player.getComponent<SpriteAnimationRenderComponent>();
    sprite->setScale(1);
    auto collider = player.getComponent<ColliderComponent>();
    collider->setScale(sf::Vector2f(1, 1));
}
} // namespace mmt_gd