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
    std::string       textureFile,
    std::string       layerName,
    sf::IntRect       textureRect,
    sf::Vector2f      frameCount) :
SpriteRenderComponent(gameObject, renderWindow, textureFile, layerName, textureRect),
m_frameCount(frameCount)
{
}

SpriteAnimationRenderComponent::~SpriteAnimationRenderComponent()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layerName, *this));
}

bool SpriteAnimationRenderComponent::init()
{
    sf::Image image;
    if (!image.loadFromFile(m_textureFile))
    {
        sf::err() << "Could not load texture from " << m_textureFile << '\n';
        return false;
    }
    image.createMaskFromColor(sf::Color::Black);
    m_texture.loadFromImage(image);
    m_sprite.setTexture(m_texture);

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
    m_gameTime += deltaTime;
    if (m_lastFramePos != m_gameObject.getPosition())
    {
        auto dif = m_gameObject.getPosition() - m_lastFramePos;
        if (dif.y > 0)
        {
            m_direction = Down;
        }
        if (dif.y < 0)
        {
            m_direction = Up;
        }
        if (dif.x > 0)
        {
            m_direction = Right;
        }
        if (dif.x < 0)
        {
            m_direction = Left;
        }
    }
    auto        textureRect    = m_texture.getSize();
    int         animationFrame = static_cast<int>(m_gameTime) % static_cast<int>(m_frameCount.x);
    sf::IntRect newRect((textureRect.x / m_frameCount.x) * animationFrame + m_textureRect.left,
                        (textureRect.y / m_frameCount.y) * static_cast<int>(m_direction) + m_textureRect.top,
                        m_textureRect.width,
                        m_textureRect.height);
    m_sprite.setTextureRect(newRect);
}

void SpriteAnimationRenderComponent::draw()
{
    SpriteRenderComponent::draw();
}
} // namespace mmt_gd