#pragma once

#include "ColliderComponent.hpp"
#include "DeadComponent.h"
#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "SoundComponent.hpp"

#include <functional>
#include <vector>

namespace mmt_gd
{
class PlayerMoveComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<PlayerMoveComponent>;

    PlayerMoveComponent(GameObject&         gameObject,
                        RigidBodyComponent& rigidBody,
                        DeadComponent&      deadComponent,
                        SoundComponent&     soundComponent,
                        int                 playerIndex = 0);

    float getDashDuration() const
    {
        return m_dashDuration;
    }

    bool init() override;
    void update(float deltaTime) override;

    void OnCollision();
    void subscribeToOnDash(std::function<void()> subscriber)
    {
        m_onDash.push_back(subscriber);
    }
    void subscribeToOnDashEnd(std::function<void()> subscriber)
    {
        m_onDashEnd.push_back(subscriber);
    }

private:
    DeadComponent&      m_deadComponent;
    RigidBodyComponent& m_rigidBody;
    SoundComponent&     m_soundComponent;
    sf::Vector2f        m_lastMoveDirection;

    int   m_playerIndex;
    bool  m_isDashing    = false;
    bool  m_canDash      = true;
    float m_dashCooldown = 0.f;
    float m_dashDuration = 0.f;

    std::vector<std::function<void()>> m_onDash;
    std::vector<std::function<void()>> m_onDashEnd;
};
} // namespace mmt_gd