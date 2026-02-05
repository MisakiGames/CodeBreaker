#pragma once
#include "GameObject.hpp"
#include "SpriteManager.hpp"
#include "Tileson.hpp"
#include "GameObjectManager.hpp"

namespace tson
{
class Object;
}

namespace mmt_gd
{
class ObjectFactory
{
public:
    static GameObject::Ptr processTsonObject(tson::Object&        object,
                                             const tson::Layer&   layer,
                                             const fs::path&      path,
                                             const SpriteManager& spriteManager,
                                             GameObjectManager&    goManager);
};
} //namespace mmt_gd
