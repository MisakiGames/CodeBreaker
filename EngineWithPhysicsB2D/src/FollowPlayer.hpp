#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
class FollowPlayer : public IComponent
{
public:
    FollowPlayer(GameObject& self, GameObject& player) : IComponent(self), m_player(player) {};
    bool init()
    {
        return true;
    }
    void update(float deltaTime);

private:
    GameObject& m_player;
};
} // namespace mmt_gd
