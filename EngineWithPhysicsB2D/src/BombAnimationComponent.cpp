#include "stdafx.h"

#include "BombAnimationComponent.h"
#include "EventBus.hpp"
#include "GameObject.hpp"
#include "RenderComponentEvents.hpp"

namespace mmt_gd
{
BombAnimationComponent::BombAnimationComponent(
    GameObject&       gameObject,
    sf::RenderWindow& renderWindow,
    std::string       texturePath,
    std::string       layerName,
    float             speed,
    sf::IntRect       m_textureRect,
    sf::Vector2f      frameCount):
IRenderComponent(gameObject, renderWindow),
m_textureFile(std::move(texturePath)),
m_layerName(std::move(layerName)),
m_textureRect(m_textureRect),
m_hasTextureRect(m_textureRect.width > 0 && m_textureRect.height > 0),m_speed(speed),m_frameCount(frameCount)
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(m_layerName, *this));
}
BombAnimationComponent::~BombAnimationComponent()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layerName, *this));
}
bool BombAnimationComponent::init()
{
    if (!m_texture.loadFromFile(m_textureFile))
    {
        sf::err() << "Could not load texture from " << m_textureFile << '\n';
        return false;
    }
    m_sprite.setTexture(m_texture);
    // Apply texture rect AFTER setTexture (which resets to full texture)
    if (m_hasTextureRect)
    {
        m_sprite.setTextureRect(m_textureRect);
    }
    return true;
}
void BombAnimationComponent::update(float deltaTime)
{
    if (m_stop)
        return;

    m_time += deltaTime * m_speed;
    int  animationFrame = static_cast<int>(m_time) % static_cast<int>(m_frameCount.x);
    if (animationFrame == m_frameCount.x - 1)
        m_stop = true;
    sf::IntRect newRect(m_textureRect.getSize().x * animationFrame,
                        m_textureRect.getPosition().y,
                        m_textureRect.width,
                        m_textureRect.height);
    m_sprite.setTextureRect(newRect);

}
void BombAnimationComponent::draw()
{
    // TODO: adapt transform to have pixel precise movement

    m_renderWindow.draw(m_sprite, m_gameObject.getTransform());
}
} // namespace mmt_gd