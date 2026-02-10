//comment
#pragma once

#include "ItemComponent.h"
namespace mmt_gd
{
class ResizeItemComponent : public ItemComponent
{
public:
    ResizeItemComponent(GameObject& gameObject, ItemType type, float maxTime) :
    ItemComponent(gameObject, type, maxTime) {};
    void use(GameObject& player) override;
    void stopUse(GameObject& player) override;
    void update(float deltaTime) override;

private:
    float       m_newScale = 3;
    float       m_oldScale = 1;
    GameObject* m_player   = nullptr;
    bool        m_big      = false;
    bool        m_resize   = false;
};
} // namespace mmt_gd
