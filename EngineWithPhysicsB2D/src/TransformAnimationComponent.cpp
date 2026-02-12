//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.hpp"

#include "TransformAnimationComponent.hpp"

#include "GameObject.hpp"

#include <utility>

namespace mmt_gd
{
TransformAnimationComponent::TransformAnimationComponent(GameObject& gameObject, ITransformAnimation::Ptr animation) :
IAnimationComponent(gameObject),
m_animation(std::move(animation))
{
}

bool TransformAnimationComponent::init()
{
    if (m_animation == nullptr)
    {
        FF_ERROR_MSG("TransformAnimation must not be nullptr") return false;
    }
    return true;
}

void TransformAnimationComponent::update(float fDeltaTime)
{
    ffAssertMsg(m_animation != nullptr, "Animation must not be nullptr")

        m_animation->update(fDeltaTime, m_gameObject);
}
} // namespace mmt_gd
