#pragma once

#include "GameObject.hpp"
#include "GameObjectManager.hpp"

#include <SFML/Graphics.hpp>
namespace mmt_gd
{
class PlayerFactory
{
public:
    static GameObject::Ptr createPlayer(sf::RenderWindow&  window,
                                        enum PlayerSpawn   spawnName,
                                        GameObjectManager& goManager,
                                        int                plrIndex,
                                        std::string        color,
                                        const std::unordered_map<std::string, sf::SoundBuffer>& buffers);
};

enum PlayerSpawn
{
    TopLeft,
    BottomLeft,
    TopRight,
    BottomRight
};
} // namespace mmt_gd