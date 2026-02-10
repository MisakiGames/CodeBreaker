//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#pragma once
#include <SFML/Graphics.hpp>
#include "IComponent.hpp"

namespace mmt_gd
{
class RectComponent : public IComponent
{
public:
    RectComponent(GameObject& gameObject, sf::IntRect rect) : IComponent(gameObject), m_rect(rect) {}; 
    sf::IntRect m_rect;
    bool        init()
    {
        m_rect.left = m_gameObject.getPosition().x;
        m_rect.top  = m_gameObject.getPosition().y;
        return true;
    }
    void        update(float deltaTime)
    {
        m_rect.left = m_gameObject.getPosition().x;
        m_rect.top= m_gameObject.getPosition().y;
    }
};
} // namespace mmt_gd
