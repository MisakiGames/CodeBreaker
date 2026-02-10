//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "TransformAnimationConstantMotion.hpp"

namespace mmt_gd
{
void TransformAnimationConstantMotion::update(const float deltaTime, sf::Transformable& transform)
{
    transform.move(m_direction * m_fSpeed * deltaTime);
}
} // namespace mmt_gd
