//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "SpriteAnimationRenderComponent.h"

#include "EventBus.hpp"
#include "GameObject.hpp"
#include "RenderComponentEvents.hpp"

namespace mmt_gd
{
SpriteAnimationRenderComponent::SpriteAnimationRenderComponent(
    GameObject&       gameObject,
    sf::RenderWindow& renderWindow,
    std::string       layerName,
    sf::IntRect       m_textureRect,
    sf::Vector2f      frameCount) :
IRenderComponent(gameObject, renderWindow),
m_frameCount(frameCount),
m_layerName(std::move(layerName)),
m_textureRect(m_textureRect),
m_hasTextureRect(m_textureRect.width > 0 && m_textureRect.height > 0)
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(m_layerName, *this));
}

SpriteAnimationRenderComponent::~SpriteAnimationRenderComponent()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layerName, *this));
}

bool SpriteAnimationRenderComponent::init()
{
    m_sprite.setTexture(m_textures[m_state]);

    // Apply texture rect AFTER setTexture (which resets to full texture)
    if (m_hasTextureRect)
    {
        m_sprite.setTextureRect(m_textureRect);
    }
    m_lastFramePos = m_gameObject.getPosition();

    return true;
}

void SpriteAnimationRenderComponent::update(float deltaTime)
{
    if (!m_stateSetThisFrame)
        setStateSecret(AnimationState::Idle);
    if (m_lastFramePos != m_gameObject.getPosition())
    {
        auto dif = m_gameObject.getPosition() - m_lastFramePos;
        if (dif.y > 0.01)
        {
            m_direction = Down;
            if (!m_stateSetThisFrame)
                setStateSecret(AnimationState::Walk);
        }
        if (dif.y < -0.01)
        {
            m_direction = Up;
            if (!m_stateSetThisFrame)
                setStateSecret(AnimationState::Walk);
        }
        if (dif.x > 0.01)
        {
            m_direction = Right;
            if (!m_stateSetThisFrame)
                setStateSecret(AnimationState::Walk);
        }
        if (dif.x < -0.01)
        {
            m_direction = Left;
            if (!m_stateSetThisFrame)
                setStateSecret(AnimationState::Walk);
        }
        m_lastFramePos = m_gameObject.getPosition();
    }

    m_time += deltaTime * m_texturesSpeed[m_state];
    m_sprite.setTexture(m_textures[m_state]);
    auto textureRect    = m_textures[m_state].getSize();
    int  animationFrame = static_cast<int>(m_time) % static_cast<int>(m_frameCount.x);
    if (!m_texturesLoops[m_state] && static_cast<int>(m_time) > m_frameCount.x - 1)
        animationFrame = m_frameCount.x - 1;
    sf::IntRect newRect((textureRect.x / m_frameCount.x) * animationFrame + m_textureRect.left,
                        (textureRect.y / m_frameCount.y) * static_cast<int>(m_direction) + m_textureRect.top,
                        m_textureRect.width,
                        m_textureRect.height);
    m_sprite.setTextureRect(newRect);
    m_stateSetThisFrame = false;
}

bool SpriteAnimationRenderComponent::loadAndMapTexture(std::string texturePath, enum AnimationState state, float speed, bool loop)
{
    sf::Texture m_texture;
    if (!m_texture.loadFromFile(texturePath))
    {
        sf::err() << "Could not load texture from " << texturePath << '\n';
        return false;
    }
    m_textures[state]      = m_texture;
    m_texturesLoops[state] = loop;
    m_texturesSpeed[state] = speed;
    return true;
}

void SpriteAnimationRenderComponent::draw()
{
    m_renderWindow.draw(m_sprite, m_gameObject.getTransform());
}
void SpriteAnimationRenderComponent::setScale(float set)
{
    m_gameObject.setScale(sf::Vector2f(set, set));
}
} // namespace mmt_gd