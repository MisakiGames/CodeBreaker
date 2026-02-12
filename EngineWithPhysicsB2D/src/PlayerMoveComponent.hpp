//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "ColliderComponent.hpp"
#include "DamageComponent.hpp"
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
                        DamageComponent&    damage,
                        int                 playerIndex = 0);

    float getDashDuration() const
    {
        return m_dashDuration;
    }

    bool init() override;
    void update(float deltaTime) override;

    void OnCollision();
    void deactivateDash()
    {
        m_dashActive = false;
    }
    void activateDash()
    {
        m_dashActive = true;
    };
    void subscribeToOnDash(std::function<void()> subscriber)
    {
        m_onDash.push_back(subscriber);
    }
    void subscribeToOnWhileDash(std::function<void()> subscriber)
    {
        m_onWhileDash.push_back(subscriber);
    }
    void subscribeToOnDashEnd(std::function<void()> subscriber)
    {
        m_onDashEnd.push_back(subscriber);
    }
    void subscribeToOnMoved(std::function<void()> subscriber)
    {
        m_onMoved.push_back(subscriber);
    }

private:
    void                DoOnMoved();
    int                 m_playerIndex;
    DeadComponent&      m_deadComponent;
    RigidBodyComponent& m_rigidBody;
    sf::Vector2f        m_lastMoveDirection;

    bool m_isDashing  = false;
    bool m_canDash    = true;
    bool m_dashActive = true;

    float m_dashCooldown = 0.f;
    float m_dashDuration = 0.f;

    std::vector<std::function<void()>> m_onDash;
    std::vector<std::function<void()>> m_onWhileDash;
    std::vector<std::function<void()>> m_onDashEnd;
    std::vector<std::function<void()>> m_onMoved;
    DamageComponent&                   m_damage;
    bool                               m_resized = false;
};
} // namespace mmt_gd