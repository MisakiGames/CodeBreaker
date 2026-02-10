//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
#pragma once

#include "IComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
class EnemyAIComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<EnemyAIComponent>;

    EnemyAIComponent(GameObject& gameObject,
                     RigidBodyComponent& rigidBody,
                     PlayerShootComponent& shootComponent,
                     GameObject* playerTarget,
                     float moveSpeed = 100.0f,
                     float shootInterval = 1.5f,
                     float minY = 100.0f,
                     float maxY = 500.0f);

    [[nodiscard]] bool init() override;
    void update(float deltaTime) override;

private:
    void updateMovement(float deltaTime);
    void updateShooting(float deltaTime);
    sf::Vector2f calculateShootDirection() const;

    RigidBodyComponent& m_rigidBody;
    PlayerShootComponent& m_shootComponent;
    GameObject* m_playerTarget;

    float m_moveSpeed;
    float m_shootInterval;
    float m_timeSinceLastShot;

    float m_minY;
    float m_maxY;
    int m_direction;
};
} // namespace mmt_gd
