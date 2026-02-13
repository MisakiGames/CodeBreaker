//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "SpriteManager.hpp"
#include "TileLayerRenderComponent.hpp"
#include "Tileson.hpp"
#include "GameObjectManager.hpp"

namespace mmt_gd
{
class TileMapLoader
{
public:
    static void loadTileSetTextures(const std::unique_ptr<tson::Map>& map,
                                    const fs::path&                   resourcePath,
                                    SpriteManager&                    spriteManager,
                                    SpriteManager::TileSetMap&        tileSets);

    static void createTileSprite(const std::unique_ptr<tson::Map>& map,
                                 SpriteManager::TileSetMap&        tileSets,
                                 std::vector<TileLayer>&           tileLayers,
                                 int                               layerIdx,
                                 const tson::Layer&                layer,
                                 int                               i,
                                 unsigned                          gid);

    static void loadTileLayers(const std::unique_ptr<tson::Map>& map, const fs::path& resourcePath, SpriteManager& spriteManager);

    static void loadObjectLayers(const std::unique_ptr<tson::Map>& map,
                                 const fs::path&                   resourcePath,
                                 const SpriteManager&              spriteManager,
                                 GameObjectManager&                goManager);
};
} // namespace mmt_gd
