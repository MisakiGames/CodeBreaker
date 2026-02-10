//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
#pragma once

#include "IComponent.hpp"
namespace mmt_gd
{
class PlayerUIComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<PlayerUIComponent>;

    PlayerUIComponent(GameObject& gameObject);

    bool init() override;
    void update(float deltaTime) override;

    void setPosition(sf::Vector2f position);
    void setColor(sf::Color color);
    void setHealth(float health);
    void setScore(float score);

private:
    sf::Vector2f m_position;
    sf::Color    m_color;
    float        m_health;
    float        m_score;

    const tgui::Panel   m_panel;
    const tgui::Canvas::Ptr   m_colorCanvas;
    const tgui::ProgressBar::Ptr m_healthBar;
    const tgui::Label         m_scoreLabel;
};
} // namespace mmt_gd
