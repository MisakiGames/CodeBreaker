#pragma once

#include "HealthComponent.hpp"
#include "IComponent.hpp"
#include "RespawnComponent.h"

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

private:
    HealthComponent&  m_healthComponent;
    RespawnComponent& m_respawnComponent;
    bool              m_dead;
    float             m_time;
    float             m_maxTime;
};
} // namespace mmt_gd