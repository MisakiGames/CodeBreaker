//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
#pragma once
namespace mmt_gd
{
class ITransformAnimation
{
public:
    using Ptr = std::shared_ptr<ITransformAnimation>;

    virtual ~ITransformAnimation() = default;

    virtual void update(float deltaTime, sf::Transformable& transform) = 0;
};
} // namespace mmt_gd
