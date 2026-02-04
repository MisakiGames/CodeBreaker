#pragma once

#include "ItemComponent.h"
namespace mmt_gd
{

class BombItemComponent: public ItemComponent
{

public:
    BombItemComponent(GameObject& gameObject, ItemType type, float maxTime, std::shared_ptr<GameObject> bomb) :
    ItemComponent(gameObject, type, maxTime),
    m_bomb(bomb){};
    void use(GameObject& player) override;
    void stopUse(GameObject& player) override;

private:
    GameObject* m_player = nullptr;
    std::shared_ptr<GameObject> m_bomb;
};
} // namespace mmt_gd
