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
    sf::View m_view;
    sf::Vector2f m_standardPosition = { 0.f, 0.f };
    std::vector<std::shared_ptr<RigidBodyComponent>> m_targets;
};
} // namespace mmt_gd
