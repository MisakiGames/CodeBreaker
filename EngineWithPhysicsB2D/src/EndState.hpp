//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "GameState.hpp"
#include <SFML/Graphics.hpp>

namespace mmt_gd
{
class EndState : public GameState
{
public:
    EndState(GameStateManager* gameStateManager, Game* game);
    void init() override;
    void exit() override;
    void update(float deltaTime) override;
    void draw() override;

private:
    sf::View m_view;
};
} // namespace mmt_gd
