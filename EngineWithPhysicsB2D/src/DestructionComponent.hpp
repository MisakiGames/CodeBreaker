//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
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
    void               setPosition(sf::Vector2f setPos);
    bool             warp = false;
    sf::Vector2f     warpTo{};
    HealthComponent& m_healthComponent;
    bool m_destroyed;
};
} // namespace mmt_gd
