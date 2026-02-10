//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once

#include "IComponent.hpp"

namespace mmt_gd
{
class IAnimationComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<IAnimationComponent>;

    using IComponent::IComponent;
};
} // namespace mmt_gd
