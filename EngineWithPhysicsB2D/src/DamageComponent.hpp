//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "IComponent.hpp"

#include <string>
#include <vector>

namespace mmt_gd
{
class DamageComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<DamageComponent>;

    DamageComponent(GameObject& gameObject, float damage, const std::string& ownerId);

    [[nodiscard]] bool init() override;
    void               update(float deltaTime) override;

    [[nodiscard]] int         getDamage();
    [[nodiscard]] std::string getOwnerId() const;
    bool                      isActive()
    {
        return m_isActive;
    };
    void setActive(bool set)
    {
        m_isActive = set;
    };
    void setBaseDamage(float damage);

    void setOwnerId(const std::string& ownerId);
    void addMultiplier(float add)
    {
        m_multipliers.push_back(add);
    }

private:
    float         m_baseDamage;
    std::string m_ownerId;
    bool        m_isActive     = true;
    std::vector<float> m_multipliers;
};
} // namespace mmt_gd
