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
        std::cout << m_score << std::endl;
        if (m_hasCrown)
            m_score += deltaTime;
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

private:
    bool  m_hasCrown = false;
    float m_score    = 0;
};
} // namespace mmt_gd
