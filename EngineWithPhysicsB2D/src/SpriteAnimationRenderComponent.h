#pragma once

#include "IRenderComponent.hpp"


#include <SFML/Graphics.hpp>
#include <string>
#include <map>
namespace mmt_gd
{
enum class AnimationState
{
    Idle,
    Walk,
    Dead,
    Dash
};

class SpriteAnimationRenderComponent : public IRenderComponent
{
public:
    using Ptr = std::shared_ptr<SpriteAnimationRenderComponent>;

    SpriteAnimationRenderComponent(GameObject&       gameObject,
                                   sf::RenderWindow& renderWindow,
                                   std::string       layerName,
                                   sf::IntRect       textureRect = sf::IntRect(),
                                   sf::Vector2f      frameCount  = sf::Vector2f(1, 1));

    ~SpriteAnimationRenderComponent() override;

    bool init() override;

    void update(float deltaTime) override;

    bool loadAndMapTexture(std::string texturePath, enum AnimationState state);

    void setState(enum AnimationState state)
    {
        setStateSecret(state);
        m_stateSetThisFrame = true;
        m_gameTime          = 0;
    }



    void draw() override;

        sf::Sprite& getSprite()
    {
        return m_sprite;
    }

    void setTextureRect(const sf::IntRect& rect)
    {
        m_textureRect    = rect;
        m_hasTextureRect = true;
    }

private:
    void setStateSecret(enum AnimationState state)
    {
        m_state = state;
    }

    enum AnimationDirection
    {
        Down  = 0,
        Left  = 1,
        Up    = 3,
        Right = 5
    };

private:
    std::string        m_textureFile;
    std::unordered_map<AnimationState,sf::Texture>       m_textures;
    sf::Sprite         m_sprite;
    std::string        m_layerName;
    sf::IntRect        m_textureRect;
    bool               m_hasTextureRect = false;
    float              m_gameTime = 0;
    sf::Vector2f       m_frameCount{}, m_lastFramePos;
    AnimationDirection m_direction;
    AnimationState m_state = AnimationState::Idle;
    bool                                                 m_stateSetThisFrame = false;
};


} // namespace mmt_gd

