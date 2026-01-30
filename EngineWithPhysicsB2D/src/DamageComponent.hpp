#pragma once

#include "IComponent.hpp"
#include <string>

namespace mmt_gd
{
class DamageComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<DamageComponent>;

    DamageComponent(GameObject& gameObject, int damage, const std::string& ownerId);

    [[nodiscard]] bool init() override;
    void update(float deltaTime) override;

    [[nodiscard]] int getDamage() const;
    [[nodiscard]] std::string getOwnerId() const;
    
    void setOwnerId(const std::string& ownerId);

private:
    int m_damage;
    std::string m_ownerId;
};
} // namespace mmt_gd
