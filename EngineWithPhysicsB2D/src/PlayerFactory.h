#pragma once

#include "GameObject.hpp"
#include "GameObjectManager.hpp"

#include <SFML/Graphics.hpp>
namespace mmt_gd
{
class PlayerFactory
{
public:
    static GameObject::Ptr createPlayer(sf::RenderWindow& window, enum PlayerSpawn spawnName, GameObjectManager& goManager);
};

enum PlayerSpawn
{
    TopLeft,
    BottomLeft,
    TopRight,
    BottomRight
};
} // namespace mmt_gd