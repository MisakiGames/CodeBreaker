#pragma once

#include "ItemComponent.h"
namespace mmt_gd
{
class BombItemComponent : public ItemComponent
{
public:
    BombItemComponent(GameObject& gameObject, ItemType type, float maxTime, std::shared_ptr<GameObject> bomb) :
    ItemComponent(gameObject, type, maxTime),
    m_bomb(bomb) {};
    void use(GameObject& player) override;
    void stopUse(GameObject& player) override;
    void update(float deltaTime) override;

private:
    void                        explode();
    GameObject*                 m_player = nullptr;
    std::shared_ptr<GameObject> m_bomb;
    float                       m_spawnTime    = 0;
    float                       m_maxSpawnTime = 3;
};
} // namespace mmt_gd
