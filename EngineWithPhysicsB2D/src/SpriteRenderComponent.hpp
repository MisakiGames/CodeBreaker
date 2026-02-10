//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once

#include "IRenderComponent.hpp"

#include <SFML/Graphics.hpp>
#include <string>

namespace mmt_gd
{
class SpriteRenderComponent : public IRenderComponent
{
public:
    using Ptr = std::shared_ptr<SpriteRenderComponent>;

    SpriteRenderComponent(GameObject&       gameObject,
                          sf::RenderWindow& renderWindow,
                          std::string       textureFile,
                          std::string       layerName,
                          sf::IntRect       m_textureRect = sf::IntRect());

    virtual ~SpriteRenderComponent() override;

    virtual bool init() override;

    virtual void update(float deltaTime) override
    {
    }

    virtual void draw() override;

    sf::Sprite& getSprite()
    {
        return m_sprite;
    }

    void setTextureRect(const sf::IntRect& rect)
    {
        m_textureRect    = rect;
        m_hasTextureRect = true;
    }

protected:
    std::string m_textureFile;
    sf::Texture m_texture;
    sf::Sprite  m_sprite;
    std::string m_layerName;
    sf::IntRect m_textureRect;
    bool        m_hasTextureRect = false;
};
} // namespace mmt_gd
