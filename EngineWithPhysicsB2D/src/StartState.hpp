//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "GameState.hpp"
#include "GameStateManager.hpp"

namespace mmt_gd
{
class StartState final : public GameState
{
public:
    StartState(GameStateManager* gameStateManager, Game* game) : GameState(gameStateManager, game)
    {
    }

    void init() override;
    void update(float delta) override;
    void draw() override;
    void exit() override;

private:
    float       m_accumulatedTime = 0.0f;
    const float m_displayDuration = 5.0f;
};
} // namespace mmt_gd