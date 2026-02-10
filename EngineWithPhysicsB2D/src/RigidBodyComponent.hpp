//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once

#include "IComponent.hpp"

#include <Box2D/box2d.h>

namespace mmt_gd
{
class RigidBodyComponent : public IComponent
{
public:
    RigidBodyComponent(GameObject& gameObject, b2BodyType type, bool allowSleep = true);
    ~RigidBodyComponent() override;

    void addVelocity(const sf::Vector2f& velocity) const;

    void setVelocity(const sf::Vector2f& velocity) const;

    bool init() override
    {
        return true;
    }

    void update(float deltaTime) override {};

    void physicsUpdate(float deltaTime);

    b2Body* getB2Body() const;

private:
    b2Body* m_body;
};
} // namespace mmt_gd