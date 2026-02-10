//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include <memory>

namespace mmt_gd
{
class GameObject;

class IComponent
{
public:
    using Ptr = std::shared_ptr<IComponent>;

    explicit IComponent(GameObject& gameObject) : m_gameObject(gameObject)
    {
    }

    virtual ~IComponent()                        = default;
    IComponent(const IComponent& other)          = default;
    IComponent(IComponent&& other) noexcept      = default;
    IComponent& operator=(const IComponent&)     = delete;
    IComponent& operator=(IComponent&&) noexcept = delete;

    [[nodiscard]] virtual bool init()                  = 0;
    virtual void update(float deltaTime) = 0;

    GameObject& getGameObject() const
    {
        return m_gameObject;
    }
    void setGameObjectDeleted()
    {
        m_gameObjectDeleted = true;
    }
    bool getGameObjectDeleted()
    {
        return m_gameObjectDeleted;
    }

protected:
    GameObject& m_gameObject;
    bool        m_gameObjectDeleted = false;
};
} // namespace mmt_gd
