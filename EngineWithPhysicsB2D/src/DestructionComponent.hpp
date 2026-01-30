#pragma once

#include "HealthComponent.hpp"
#include "IComponent.hpp"

namespace mmt_gd
{
class DestructionComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<DestructionComponent>;

    DestructionComponent(GameObject& gameObject, HealthComponent& healthComponent);

    [[nodiscard]] bool init() override;
    void update(float deltaTime) override;

private:
    void triggerDestruction();

    HealthComponent& m_healthComponent;
    bool m_destroyed;
};
} // namespace mmt_gd
