#pragma once

#include "IComponent.hpp"

#include <SFML/System.hpp>
#include <functional>
#include <vector>

namespace mmt_gd
{
class RespawnComponent : public IComponent
{
public:
    RespawnComponent(GameObject& gameObject) : IComponent(gameObject)
    {
    }
    ~RespawnComponent()
    {
        on_respawned.clear();
    }

    bool init() override;
    void update(float deltaTime) override;
    void startRespawn();
    void SubscribeToOnRespawn(std::function<void()> subscriber)
    {
        on_respawned.push_back(subscriber);
    }

private:
    sf::Vector2f                       m_spawn;
    std::vector<std::function<void()>> on_respawned;
    bool                               warp = false;
    ;
};
} // namespace mmt_gd
