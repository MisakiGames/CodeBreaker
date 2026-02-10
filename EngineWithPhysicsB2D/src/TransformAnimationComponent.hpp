//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "IAnimationComponent.hpp"
#include "ITransformAnimation.hpp"

namespace mmt_gd
{
/**
 * \brief The component controlling transform animations. The specific
 * animation pattern can be handed in via an implementation of
 * TransformAnimation.
 */
class TransformAnimationComponent final : public IAnimationComponent
{
public:
    using Ptr = std::shared_ptr<TransformAnimationComponent>;

    TransformAnimationComponent(GameObject& gameObject, ITransformAnimation::Ptr animation);

    bool init() override;
    void update(float deltaTime) override;

private:
    ITransformAnimation::Ptr m_animation;
};
} // namespace mmt_gd
