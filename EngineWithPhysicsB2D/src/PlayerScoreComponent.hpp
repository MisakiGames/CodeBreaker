//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "IComponent.hpp"

#include <iostream>

namespace mmt_gd
{
class PlayerScoreComponent : public IComponent
{
public:
    PlayerScoreComponent(GameObject& gameObject) : IComponent(gameObject) {};
    bool init() override
    {
        return true;
    }
    void update(float deltaTime) override
    {
        if (m_hasCrown)
            m_score += deltaTime * m_pointsPerSecond;
    }

    bool hasCrown()
    {
        return m_hasCrown;
    }

    void setHasCrown(bool set)
    {
        m_hasCrown = set;
    }

    void removePoints(float remove)
    {
        m_score -= remove;
        if (m_score < 0)
            m_score = 0;
    }

    float getScore() const
    {
        return m_score;
    }

private:
    bool        m_hasCrown        = false;
    float       m_score           = 0;
    const float m_pointsPerSecond = 3.f;
};
} // namespace mmt_gd