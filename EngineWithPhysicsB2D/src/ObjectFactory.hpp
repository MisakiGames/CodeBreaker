//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
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
