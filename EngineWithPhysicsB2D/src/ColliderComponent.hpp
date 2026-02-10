//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once
#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

#include <Box2D/Box2D.h>
#include <functional>
#include <list>

namespace mmt_gd
{
class ColliderComponent : public IComponent
{
public:
    /// \param collider1 the collider for which OnCollision was called.
    /// \param collider2 the collider with which the collision happened.
    using OnCollisionFunction = std::function<void(ColliderComponent&, ColliderComponent&)>;

    /// \param body the rigid body the collider is attached to.
    /// \param rect the size of the AABB in pixel
    ColliderComponent(GameObject& gameObject, RigidBodyComponent& body, b2FixtureDef& def);
    void setTag(std::string tag)
    {
        m_tag = tag;
    };
    std::string getTag()
    {
        return m_tag;
    }
    bool init() override
    {
        return true;
    }
    bool isSensor()
    {
        return fixture->IsSensor();
    }
    virtual void update(float fDeltaTime) override;
    /// Add delegate function to be executed when collision is detected.
    /// Signature: void func(ColliderComponent&)
    void registerOnCollisionEnterFunction(const OnCollisionFunction& func);
    void registerOnCollisionExitFunction(const OnCollisionFunction& func);

    /// Method called when collision occured. Method calls all subscribed
    /// OnCollisionFunctions
    ///
    /// \param collider collision occured with this collider
    void                onCollisionEnter(ColliderComponent& collider);
    void                onCollisionExit(ColliderComponent& collider);
    RigidBodyComponent& getBody() const
    {
        return m_body;
    }
    b2Fixture* getFixture()
    {
        return fixture;
    }

private:
    RigidBodyComponent&            m_body;
    b2Fixture*                     fixture;
    std::list<OnCollisionFunction> m_onCollisionEnterFunctions;
    std::list<OnCollisionFunction> m_onCollisionExitFunctions;
    std::string                    m_tag = "Collider";
};
} // namespace mmt_gd
