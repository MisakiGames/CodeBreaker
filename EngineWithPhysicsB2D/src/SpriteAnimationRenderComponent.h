//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once

#include "IRenderComponent.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
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
                                   sf::IntRect       m_textureRect = sf::IntRect(),
                                   sf::Vector2f      frameCount  = sf::Vector2f(1, 1));

    ~SpriteAnimationRenderComponent() override;

    bool init() override;

    void update(float deltaTime) override;

    bool loadAndMapTexture(std::string texturePath, enum AnimationState state, float speed = 1, bool loop = true);

    void setState(enum AnimationState state)
    {
        if (m_state != state)
            m_time = 0;
        m_state             = state;
        m_stateSetThisFrame = true;
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
    void setScale(float set);

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
    std::string                                     m_textureFile;
    std::unordered_map<AnimationState, sf::Texture> m_textures;
    std::unordered_map<AnimationState, bool>        m_texturesLoops;
    std::unordered_map<AnimationState, float>       m_texturesSpeed;
    sf::Sprite                                      m_sprite;
    std::string                                     m_layerName;
    sf::IntRect                                     m_textureRect;
    bool                                            m_hasTextureRect = false;
    float                                           m_time           = 0;
    sf::Vector2f                                    m_frameCount{}, m_lastFramePos;
    AnimationDirection                              m_direction;
    AnimationState                                  m_state             = AnimationState::Idle;
    bool                                            m_stateSetThisFrame = false;
};
} // namespace mmt_gd
