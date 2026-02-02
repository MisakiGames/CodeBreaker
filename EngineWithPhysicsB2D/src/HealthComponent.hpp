#pragma once

#include "IComponent.hpp"

namespace mmt_gd
{
class HealthComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<HealthComponent>;

    HealthComponent(GameObject& gameObject, int maxHealth, float maxInvince, bool invincible = false);

    [[nodiscard]] bool init() override;
    void               update(float deltaTime) override;

    void takeDamage(int damage);
    void heal(int amount);

    [[nodiscard]] bool isAlive() const;
    [[nodiscard]] bool isInvincible() const;
    [[nodiscard]] int  getHealth() const;
    [[nodiscard]] int  getMaxHealth() const;

    void setInvincible(bool invincible);

private:
    float m_invinceTime;
    float m_maxInvinceTime;
    int   m_maxHealth;
    int   m_currentHealth;
    bool  m_invincible;
};
} // namespace mmt_gd