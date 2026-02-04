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
    void               fullHealth()
    {
        m_currentHealth = m_maxHealth;
    }
    void setInvincible(bool invincible);
    void kill()
    {
        m_currentHealth = -1;
    }

private:
    float m_invinceTime = 0;
    float m_maxInvinceTime;
    int   m_maxHealth;
    int   m_currentHealth;
    bool  m_invincible;
};
} // namespace mmt_gd