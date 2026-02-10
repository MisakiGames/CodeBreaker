//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once

#include "ItemComponent.h"

#include <vector>
#include <functional>

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
    void subscribeToOnExplode(std::function<void()> sub)
    {
        m_onExplode.push_back(sub);
    }

private :
    void                        explode();
    GameObject*                 m_player = nullptr;
    std::shared_ptr<GameObject> m_bomb;
    float                       m_spawnTime    = 0;
    float                       m_maxSpawnTime = 3;
    std::vector<std::function<void()>> m_onExplode;
};
} // namespace mmt_gd
