//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "IRenderComponent.hpp"
#include <string>
#include <SFML/Graphics.hpp>

namespace mmt_gd
{
struct Tile
{
    /**
     * \brief original tile index
     */
    int                         m_idx;
    std::shared_ptr<sf::Sprite> m_sprite;
};

struct TileLayer
{
    std::string       m_name;
    sf::Vector2i      m_dimension;
    sf::Vector2i      m_tileSize;
    std::vector<Tile> m_tiles;
};

class TileLayerRenderComponent final : public IRenderComponent
{
public:
    using Ptr = std::shared_ptr<TileLayerRenderComponent>;

    TileLayerRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, TileLayer layer);
    ~TileLayerRenderComponent() override;

    bool init() override;
    void update(float deltaTime) override;
    void draw() override;

    const TileLayer& getLayer() const
    {
        return m_layer;
    }

private:
    TileLayer m_layer;

    std::list<Tile> m_visibleTiles;
};
} // namespace mmt_gd
