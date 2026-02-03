#include "stdafx.h"

#include "CameraRenderComponent.hpp"

#include "GameObject.hpp"
#include "RigidbodyComponent.hpp"
#include "PhysicsManager.hpp"

namespace mmt_gd
{
CameraRenderComponent::CameraRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, const sf::View& view) :
IRenderComponent(gameObject, renderWindow),
m_view(view)
{
}

bool CameraRenderComponent::init()
{
    return true;
}

void CameraRenderComponent::draw()
{
}

void CameraRenderComponent::setTargets(const std::vector<std::shared_ptr<GameObject>>& targets)
{
    m_targets.clear();
    for (const auto& target : targets)
    {
        auto rigidBody = target->getComponent<RigidBodyComponent>();
        if (rigidBody)
        {
            m_targets.push_back(rigidBody);
        }
    }
}

void CameraRenderComponent::update(float deltaTime)
{
    if (m_targets.empty())
    {
        m_view.setCenter(m_standardPosition);
        m_renderWindow.setView(m_view);
        return;
    }

    sf::Vector2f playerCenter(0.f, 0.f);
    for (const auto& target : m_targets)
    {
        const b2Vec2& b2Pos = target->getB2Body()->GetPosition();
        playerCenter += PhysicsManager::b2s(b2Pos);
    }

    sf::Vector2f finalCenter = playerCenter / static_cast<float>(m_targets.size());

    //m_gameObject.setPosition(finalCenter);

    m_view.setCenter(finalCenter);
    m_renderWindow.setView(m_view);
}
} // namespace mmt_gd
