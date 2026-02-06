#pragma once

#include "GameState.hpp"
#include "GameObjectManager.hpp"
#include <SFML/Graphics/Text.hpp>

namespace mmt_gd
{
class MenuState final : public GameState
{
public:
    using GameState::GameState;

    void init() override;

    void update(float delta) override;
    void draw() override;
    void exit() override;

private:
    sf::View m_view;
    GameObjectManager m_gameObjectManager;
    sf::Sprite m_backgroundSprite;
    sf::Texture m_backgroundTexture;
};
} // namespace mmt_gd
