#pragma once
#include "BulletPool.hpp"
#include "IComponent.hpp"


namespace mmt_gd
{
class PlayerShootComponent final : public IComponent
{
public:
    PlayerShootComponent(GameObject&        gameObject,
                         size_t             poolSize,
                         sf::RenderWindow&  renderWindow,
                         const std::string& textureFile,
                         const sf::IntRect& m_textureRect,
                         const sf::FloatRect& colliderRect,
                         float              bulletMass,
                         const std::string& layerName,
                         bool               useInput = true);


    void shoot(sf::Vector2f directionSpeed);
    bool init() override;
    void update(float deltaTime) override;

    BulletPool m_pool;
    float      m_timeSinceLastShot{};
    bool       m_useInput{true};
};
} // namespace mmt_gd
