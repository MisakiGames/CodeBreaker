#pragma once

#include "IRenderComponent.hpp"

namespace mmt_gd
{

class BombAnimationComponent : public IRenderComponent
{
public:
    using Ptr = std::shared_ptr<BombAnimationComponent>;

    BombAnimationComponent(GameObject&       gameObject,
                           sf::RenderWindow& renderWindow,
                           std::string       texturePath,
                           std::string       layerName,
                           float             speed         = 1,
                           sf::IntRect       m_textureRect = sf::IntRect(),
                           sf::Vector2f      frameCount    = sf::Vector2f(1, 1));

    ~BombAnimationComponent() override;

    bool init() override;

    void update(float deltaTime) override;
    void draw() override;
    void setScale(float set);
    void resetAnimation()
    {
        m_stop = false;
        m_time = 0;
    }

private:


private:
    float        m_speed;
    float        m_time = 0;
    sf::Vector2f m_frameCount{};
    std::string  m_textureFile;
    sf::Texture  m_texture;
    sf::Sprite   m_sprite;
    std::string  m_layerName;
    sf::IntRect  m_textureRect;
    bool         m_hasTextureRect = false;
    bool         m_stop = false;
};
} // namespace mmt_gd
