//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "CameraRenderComponent.hpp"

#include "GameObject.hpp"
#include "PhysicsManager.hpp"
#include "RigidbodyComponent.hpp"

namespace mmt_gd
{
CameraRenderComponent::CameraRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, const sf::View& view) :
IRenderComponent(gameObject, renderWindow),
m_view(view),
m_baseSize(view.getSize())
{
    m_view.setCenter(m_standardPosition);
    m_renderWindow.setView(m_view);
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
        m_currentZoom     = m_standardZoom;
        m_currentPosition = m_standardPosition;
    }
    else
    {
        sf::Vector2f playerCenter(0.f, 0.f);
        float        minX = FLT_MAX;
        float        maxX = -FLT_MAX;
        float        minY = FLT_MAX;
        float        maxY = -FLT_MAX;

        for (const auto& target : m_targets)
        {
            auto b2Pos = target->getB2Body()->GetPosition();
            auto pos   = PhysicsManager::b2s(b2Pos);

            minX = std::min(minX, pos.x);
            maxX = std::max(maxX, pos.x);
            minY = std::min(minY, pos.y);
            maxY = std::max(maxY, pos.y);

            playerCenter += pos;
        }
        //This function is more of a action camera:
        //playerCenter = playerCenter / static_cast<float>(m_targets.size());

        //This function assures that the camera always keeps all players in view:
        float width      = maxX - minX;
        float height     = maxY - minY;
        auto  a          = maxX - (width / 2);
        auto  b          = maxY - (height / 2) + m_padding / 4;
        playerCenter = sf::Vector2f(a, b);

        // Zoom calculation
        width      += m_padding;
        height     += m_padding;
        float zoomFactor = std::max(width / m_baseSize.x, height / m_baseSize.y);
        zoomFactor       = std::clamp(zoomFactor, m_minZoom, m_maxZoom);

        // Lerp zoom and position changes
        m_currentZoom += (zoomFactor - m_currentZoom) * deltaTime * m_zoomSpeed;
        m_currentPosition += (playerCenter - m_currentPosition) * deltaTime * m_moveSpeed;
    }

    m_view.setSize(m_baseSize.x * m_currentZoom, m_baseSize.y * m_currentZoom);
    m_view.setCenter(m_currentPosition);
    m_renderWindow.setView(m_view);
}
} // namespace mmt_gd
