#pragma once

#include "IRenderComponent.hpp"

#include <SFML/Graphics.hpp>
#include <string>

namespace mmt_gd
{
class SpriteRenderComponent final : public IRenderComponent
{
public:
    using Ptr = std::shared_ptr<SpriteRenderComponent>;

    SpriteRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, std::string textureFile, std::string layerName,
                          sf::IntRect textureRect = sf::IntRect());

    ~SpriteRenderComponent() override;

    bool init() override;

    void update(float deltaTime) override
    {
    }

    void draw() override;

    sf::Sprite& getSprite()
    {
        return m_sprite;
    }

    void setTextureRect(const sf::IntRect& rect)
    {
        m_textureRect = rect;
        m_hasTextureRect = true;
    }

private:
    std::string m_textureFile;
    sf::Texture m_texture;
    sf::Sprite  m_sprite;
    std::string m_layerName;
    sf::IntRect m_textureRect;
    bool m_hasTextureRect = false;
};
} // namespace mmt_gd
