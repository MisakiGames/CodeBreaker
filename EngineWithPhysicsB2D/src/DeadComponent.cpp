//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "DeadComponent.h"

#include "InputManager.hpp"

namespace mmt_gd
{
DeadComponent::DeadComponent(GameObject&       gameObject,
                             HealthComponent&  healthComponent,
                             RespawnComponent& respawnComponent,
                             float             maxTime) :
IComponent(gameObject),
m_healthComponent(healthComponent),
m_respawnComponent(respawnComponent),
m_maxTime(maxTime),
m_time(0),
m_dead(false)
{
}
bool DeadComponent::init()
{
    return true;
}
void DeadComponent::update(float deltaTime)
{
    if (m_healthComponent.isAlive() && !m_dead)
        return;
    m_dead = true;
    for (auto sub : m_onDeath)
        sub();
    m_time += deltaTime;
    if (m_time >= m_maxTime)
    {
        m_respawnComponent.startRespawn();
        m_dead = false;
        m_time = 0;
    }
}
} // namespace mmt_gd
