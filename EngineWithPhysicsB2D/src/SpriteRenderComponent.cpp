//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "SpriteRenderComponent.hpp"

#include "EventBus.hpp"
#include "GameObject.hpp"
#include "RenderComponentEvents.hpp"

#include <utility>

namespace mmt_gd
{
SpriteRenderComponent::SpriteRenderComponent(GameObject&       gameObject,
                                             sf::RenderWindow& renderWindow,
                                             std::string       textureFile,
                                             std::string       layerName,
                                             sf::IntRect       m_textureRect) :
IRenderComponent(gameObject, renderWindow),
m_textureFile(std::move(textureFile)),
m_layerName(std::move(layerName)),
m_textureRect(m_textureRect),
m_hasTextureRect(m_textureRect.width > 0 && m_textureRect.height > 0)
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(m_layerName, *this));
}

SpriteRenderComponent::~SpriteRenderComponent()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layerName, *this));
}

bool SpriteRenderComponent::init()
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

    return true;
}

void SpriteRenderComponent::draw()
{
    // TODO: adapt transform to have pixel precise movement

    m_renderWindow.draw(m_sprite, m_gameObject.getTransform());
}
} // namespace mmt_gd
