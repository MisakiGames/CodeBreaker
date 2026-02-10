//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "IRenderComponent.hpp"
#include "RigidBodyComponent.hpp"
#include <SFML/Graphics.hpp>

namespace mmt_gd
{
/**
 * \brief Updates the camera/view of the render window. This new camera
 * will be used until another CameraRenderComponent changes it.
 * For positioning the translation component of the game object
 * transform is used.
 * Currently does not support rotation.
 */
class CameraRenderComponent final : public IRenderComponent
{
public:
    using Ptr = std::shared_ptr<CameraRenderComponent>;

    CameraRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, const sf::View& view);

    bool init() override;
    void update(float deltaTime) override;
    void draw() override;

    void setTargets(const std::vector<std::shared_ptr<GameObject>>& targets);

private:
    sf::Vector2f m_baseSize;
    sf::View m_view;
    std::vector<std::shared_ptr<RigidBodyComponent>> m_targets;

    const sf::Vector2f m_standardPosition   = {-150.f, 150.f};
    const float  m_standardZoom       = 1.f;
    const float  m_moveSpeed       = 5.f;
    const float  m_zoomSpeed        = 6.f;
    const float  m_maxZoom         = 1.8f;
    const float  m_minZoom         = 0.2f;
    const float  m_padding            = 200.f;

    sf::Vector2f m_currentPosition = m_standardPosition;
    float        m_currentZoom     = m_standardZoom;
};
} // namespace mmt_gd
