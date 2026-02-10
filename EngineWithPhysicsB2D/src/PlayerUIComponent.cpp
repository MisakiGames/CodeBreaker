//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "PlayerUiComponent.hpp"

namespace mmt_gd
{

PlayerUIComponent::PlayerUIComponent(GameObject& gameObject) : IComponent(gameObject)
{
}

bool PlayerUIComponent::init()
{
    m_panel       = tgui::Panel::create();
    m_colorCanvas = tgui::Canvas::create({100, 100});
    m_healthBar   = tgui::ProgressBar::create();
    m_scoreLabel  = tgui::Label::create();
}

void PlayerUIComponent::update(float deltaTime)
{

}

void PlayerUIComponent::setPosition(sf::Vector2f position)
{
    m_position = position;
}

void PlayerUIComponent::setColor(sf::Color color)
{
    m_color = color;
}

void PlayerUIComponent::setHealth(float health)
{
    m_health = health;
}

void PlayerUIComponent::setScore(float score)
{
    m_score = score;
}

} // namespace mmt_gd
