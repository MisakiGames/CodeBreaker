#pragma once
#include "IComponent.hpp"

#include <functional>
#include <vector>

namespace mmt_gd
{
enum class ItemType
{
    None,
    Crown
};
class ItemComponent : public IComponent
{
public:
    ItemComponent(GameObject& gameObject, ItemType type);
    bool init() override
    {
        return true;
    };
    void update(float deltaTime) override {};
    void disappear();
    void subscribeToOnDisappear(std::function<void()> subscriber)
    {
        m_onDisappear.push_back(subscriber);
    };
    ItemType getType()
    {
        return m_type;
    };

private:
    ItemType                           m_type;
    std::vector<std::function<void()>> m_onDisappear;
};
} // namespace mmt_gd
