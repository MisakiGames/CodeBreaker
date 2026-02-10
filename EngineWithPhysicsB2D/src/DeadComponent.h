//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "HealthComponent.hpp"
#include "IComponent.hpp"
#include "RespawnComponent.h"

#include <functional>
#include <vector>

namespace mmt_gd
{
class DeadComponent : public IComponent
{
public:
    DeadComponent(GameObject&       gameObject,
                  HealthComponent&  healthComponent,
                  RespawnComponent& respawnComponent,
                  float             maxTime = 2);

    [[nodiscard]] bool init() override;
    void               update(float deltaTime) override;
    bool               isDead()
    {
        return m_dead;
    };
    void subscribeToDeath(std::function<void()> subscriber)
    {
        m_onDeath.push_back(subscriber);
    }
    void setAlive()
    {
        m_dead = false;
        m_time = 0;
    }

private:
    HealthComponent&                   m_healthComponent;
    RespawnComponent&                  m_respawnComponent;
    bool                               m_dead;
    float                              m_time;
    float                              m_maxTime;
    std::vector<std::function<void()>> m_onDeath;
};
} // namespace mmt_gd
