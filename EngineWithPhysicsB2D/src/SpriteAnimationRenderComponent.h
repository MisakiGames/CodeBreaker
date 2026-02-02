#pragma once

#include "IRenderComponent.hpp"
#include "SpriteRenderComponent.hpp"

#include <SFML/Graphics.hpp>
#include <string>
namespace mmt_gd
{
class SpriteAnimationRenderComponent : public SpriteRenderComponent
{
public:
    using Ptr = std::shared_ptr<SpriteAnimationRenderComponent>;

    SpriteAnimationRenderComponent(GameObject&       gameObject,
                                   sf::RenderWindow& renderWindow,
                                   std::string       textureFile,
                                   std::string       layerName,
                                   sf::IntRect       textureRect = sf::IntRect(),
                                   sf::Vector2f      frameCount  = sf::Vector2f(1, 1));

    ~SpriteAnimationRenderComponent() override;

    bool init() override;

    void update(float deltaTime) override;

    void draw() override;

private:
    enum AnimationDirection
    {
        Down  = 0,
        Left  = 1,
        Up    = 3,
        Right = 5
    };

private:
    float              m_gameTime = 0;
    sf::Vector2f       m_frameCount{}, m_lastFramePos;
    AnimationDirection m_direction;
};
} // namespace mmt_gd