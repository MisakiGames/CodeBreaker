//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "IComponent.hpp"
#include <vector>
#include <functional>

namespace mmt_gd
{
class HealthComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<HealthComponent>;

    HealthComponent(GameObject& gameObject, int maxHealth, float maxInvince, bool invincible = false);

    [[nodiscard]] bool init() override;
    void               update(float deltaTime) override;

    void takeDamage(const float damage);
    void takeDamageSecretly(const float damage);
    void setDamagePerSecond(float damage);
    void heal(int amount);

    [[nodiscard]] bool isAlive() const;
    [[nodiscard]] bool isInvincible() const;
    [[nodiscard]] int  getHealth() const;
    [[nodiscard]] int  getMaxHealth() const;
    void               fullHealth()
    {
        m_currentHealth = m_maxHealth;
    }
    void healPercent(float percent);
    void setInvincible(bool invincible);
    void kill()
    {
        m_currentHealth = -1;
        for (auto sub : m_onTakeDamage)
            sub();
    }
    void subsribeToOnTakeDamage(std::function<void()> subscriber)
    {
        m_onTakeDamage.push_back(subscriber);
    }


private:
    float m_invinceTime = 0;
    float m_maxInvinceTime;
    float m_damagePerSec = 0;
    int   m_maxHealth;
    float m_currentHealth;
    bool  m_invincible;
    std::vector<std::function<void()>> m_onTakeDamage;

};
} // namespace mmt_gd
