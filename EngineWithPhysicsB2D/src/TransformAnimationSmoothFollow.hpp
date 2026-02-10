//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once
#include "GameObject.hpp"
#include "ITransformAnimation.hpp"

namespace mmt
{
class TransformAnimationSmoothFollow final : public mmt_gd::ITransformAnimation
{
public:
    explicit TransformAnimationSmoothFollow(std::shared_ptr<mmt_gd::GameObject> target, float smoothFactor);

    void update(float deltaTime, sf::Transformable& transform) override;

private:
    std::shared_ptr<mmt_gd::GameObject> m_target;
    float                               m_smoothFactor;
};
} // namespace mmt
