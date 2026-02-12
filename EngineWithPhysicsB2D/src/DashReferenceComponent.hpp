#pragma once
#include "IComponent.hpp"

namespace mmt_gd
{
class DashReferenceComponent : public IComponent
{
public:
    DashReferenceComponent(GameObject& self, GameObject& dash) : IComponent(self), m_dash(dash)
    {
    }
    GameObject& m_dash;
    bool        init()
    {
        return true;
    }
    void update(float deltaTime) {};
};
} // namespace mmt_gd
