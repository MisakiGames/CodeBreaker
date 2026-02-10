//comment
#pragma once
#include "GameObject.hpp"
#include <vector>

namespace mmt_gd
{
class BulletPool
{
public:
    explicit BulletPool(size_t                                                                size,
                        const std::string&                                                    textureFile,
                        sf::IntRect                                                           m_textureRect,
                        const std::string&                                                    layerName,
                        sf::RenderWindow&                                                     renderWindow,
                        const sf::FloatRect& colliderRect,
                        float                                                                 mass/*,
                        std::function<void(BoxColliderComponent& a, BoxColliderComponent& b)> collisionCallback*/);


    std::vector<GameObject::Ptr> m_pool;
    GameObject::Ptr              get();

private:
    size_t            m_counter{};
    inline static int m_globalBulletIdx{0};
    //std::function<void(BoxColliderComponent& a, BoxColliderComponent& b)> m_collisionCallback;
};
}; // namespace mmt_gd
