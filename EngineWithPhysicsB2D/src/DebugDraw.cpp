#include "stdafx.h"

#include "DebugDraw.hpp"

#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "VectorAlgebra2D.h"

namespace mmt_gd
{
DebugDraw::DebugDraw() : m_enabled(false)
{
    if (!m_font.loadFromFile("../assets/consolab.ttf"))
    {
        sf::err() << "Could not load font\n";
    }
}

DebugDraw::~DebugDraw() = default;
;

void DebugDraw::drawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Color& color, const float thickness)
{
    if (!m_enabled)
    {
        return;
    }

    auto       dir    = p2 - p1;
    const auto length = MathUtil::length(dir);

    if (length >= 1.0F)
    {
        dir = dir / length * thickness * 0.5F;
        const sf::Vector2f right(dir.y, -dir.x);

        auto shape = std::make_shared<sf::ConvexShape>();
        shape->setPointCount(4);
        shape->setPoint(0, p1 - right);
        shape->setPoint(1, p1 + right);
        shape->setPoint(2, p2 + right);
        shape->setPoint(3, p2 - right);
        shape->setFillColor(color);

        m_shapes.push_back(shape);
    }
}

void DebugDraw::drawArrow(const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Color& color, const float thickness)
{
    if (!m_enabled)
    {
        return;
    }

    if (const auto length = MathUtil::length(direction); length >= 1.0F)
    {
        const auto         unitDirection = direction / length;
        const sf::Vector2f right(unitDirection.y, -unitDirection.x);

        constexpr auto headSize = 5.0F;
        const auto     head     = length > headSize * 2 ? length - headSize : length * 0.5F;

        const auto p1 = origin;
        const auto p2 = origin + unitDirection * head;
        const auto p3 = origin + direction;

        auto shape = std::make_shared<sf::ConvexShape>();
        shape->setPointCount(4);

        shape->setPoint(0, p1 - right * thickness * 0.5F);
        shape->setPoint(1, p1 + right * thickness * 0.5F);
        shape->setPoint(2, p2 + right * thickness * 0.5F);
        shape->setPoint(3, p2 - right * thickness * 0.5F);
        shape->setFillColor(color);

        m_shapes.push_back(shape);

        auto headShape = std::make_shared<sf::ConvexShape>();
        headShape->setPointCount(3);

        headShape->setPoint(0, p2 - right * headSize);
        headShape->setPoint(1, p2 + right * headSize);
        headShape->setPoint(2, p3);
        headShape->setFillColor(color);

        m_shapes.push_back(headShape);
    }
}

void DebugDraw::drawCircle(const sf::Vector2f& center, const float radius, const sf::Color& outlineColor, const sf::Color& fillColor)
{
    if (!m_enabled)
    {
        return;
    }

    const auto shape = std::make_shared<sf::CircleShape>(radius);
    shape->setOutlineThickness(1.0F);
    shape->setOutlineColor(outlineColor);
    shape->setFillColor(fillColor);
    shape->setPosition(center);
    shape->setOrigin(sf::Vector2f(radius, radius));

    m_shapes.push_back(shape);
}

void DebugDraw::drawRectangle(const sf::Vector2f& position,
                              const sf::Vector2f& size,
                              const sf::Color&    outlineColor,
                              const sf::Color&    fillColor)
{
    if (!m_enabled)
    {
        return;
    }

    auto shape = std::make_shared<sf::RectangleShape>(size);
    shape->setOutlineThickness(1.0F);
    shape->setOutlineColor(outlineColor);
    shape->setFillColor(fillColor);
    shape->setPosition(position);
    shape->setOrigin(size * 0.5F);

    m_shapes.push_back(shape);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, const int32 vertexCount, const b2Color& color)
{
    const auto polygon = std::make_shared<sf::ConvexShape>();
    for (auto i = 0; i < vertexCount; i++)
    {
        polygon->setPoint(i, PhysicsManager::b2s(vertices[i]));
    }
    polygon->setOutlineColor(PhysicsManager::b2s(color));
    polygon->setOutlineThickness(1.0F);
    m_shapes.push_back(polygon);
}


void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, const int32 vertexCount, const b2Color& color)
{
    const auto polygon = std::make_shared<sf::ConvexShape>();
    for (auto i = 0; i < vertexCount; i++)
    {
        polygon->setPoint(i, PhysicsManager::b2s(vertices[i]));
    }
    polygon->setFillColor(PhysicsManager::b2s(color, 50));
    polygon->setOutlineColor(PhysicsManager::b2s(color));
    polygon->setOutlineThickness(1.0F);
    m_shapes.push_back(polygon);
}

void DebugDraw::DrawCircle(const b2Vec2& center, const float radius, const b2Color& color)
{
    const auto circle = std::make_shared<sf::CircleShape>(radius * PhysicsManager::RATIO);
    circle->setPosition(sf::Vector2f(center.x * PhysicsManager::RATIO, center.y * PhysicsManager::RATIO));
    circle->setOutlineColor(PhysicsManager::b2s(color));
    m_shapes.push_back(circle);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, const float radius, const b2Vec2& axis, const b2Color& color)
{
    //no converion in cordinates of center and upper left corner, Circle in sfml is managed by default with the center
    const auto circle = std::make_shared<sf::CircleShape>(radius * PhysicsManager::RATIO);
    circle->setFillColor(PhysicsManager::b2s(color, 50));
    circle->setOutlineColor(PhysicsManager::b2s(color));
    circle->setPosition(sf::Vector2f(center.x * PhysicsManager::RATIO, center.y * PhysicsManager::RATIO));

    // line of the circle which shows the angle
    const auto p        = center + radius * axis;
    const auto line     = std::make_shared<sf::VertexArray>(sf::Lines, 2);
    (*line)[0].position = sf::Vector2f(center.x * PhysicsManager::RATIO, center.y * PhysicsManager::RATIO);
    (*line)[1].position = sf::Vector2f(p.x * PhysicsManager::RATIO, p.y * PhysicsManager::RATIO);
    (*line)[0].color = (*line)[1].color = PhysicsManager::b2s(color);

    m_shapes.push_back(circle);
    m_shapes.push_back(line);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    const auto line     = std::make_shared<sf::VertexArray>(sf::LinesStrip, 2);
    (*line)[0].position = PhysicsManager::b2s(p1);
    (*line)[1].position = PhysicsManager::b2s(p2);
    (*line)[0].color = (*line)[1].color = PhysicsManager::b2s(color);
    m_shapes.push_back(line);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
    auto            x              = xf.p.x * PhysicsManager::RATIO;
    auto            y              = xf.p.y * PhysicsManager::RATIO;
    constexpr float lineProportion = 0.15F; // 0.15 ~ 10 pixels
    const auto      p1             = xf.p;

    //red (X axis)
    auto       p2          = p1 + lineProportion * xf.q.GetXAxis();
    const auto redLine     = std::make_shared<sf::VertexArray>(sf::LinesStrip, 2);
    (*redLine)[0].position = sf::Vector2f(p1.x * PhysicsManager::RATIO, p1.y * PhysicsManager::RATIO);
    (*redLine)[1].position = sf::Vector2f(p2.x * PhysicsManager::RATIO, p2.y * PhysicsManager::RATIO);
    (*redLine)[0].color = (*redLine)[1].color = sf::Color::Red;

    //green (Y axis)
    p2                       = p1 - lineProportion * xf.q.GetYAxis();
    const auto greenLine     = std::make_shared<sf::VertexArray>(sf::LinesStrip, 2);
    (*greenLine)[0].position = sf::Vector2f(p1.x * PhysicsManager::RATIO, p1.y * PhysicsManager::RATIO);
    (*greenLine)[1].position = sf::Vector2f(p2.x * PhysicsManager::RATIO, p2.y * PhysicsManager::RATIO);
    (*greenLine)[0].color = (*greenLine)[1].color = sf::Color::Green;

    m_shapes.push_back(redLine);
    m_shapes.push_back(greenLine);
}

void DebugDraw::drawPolygonShape(const b2PolygonShape& b2Shape, const GameObject& trans, const b2Color& color)
{
    const auto convex = std::make_shared<sf::ConvexShape>();
    convex->setPointCount(b2Shape.m_count);

    for (int i = 0; i < b2Shape.m_count; i++)
    {
        convex->setPoint(i, PhysicsManager::b2s(b2Shape.m_vertices[i]));
    }
    convex->setPosition(trans.getPosition());
    convex->setRotation(trans.getRotation());
    convex->setOutlineColor(PhysicsManager::b2s(color));
    convex->setOutlineThickness(1);
    convex->setFillColor(PhysicsManager::b2s(color, 80));
    m_shapes.push_back(convex);
}

void DebugDraw::DrawPoint(const b2Vec2& p, const float size, const b2Color& color)
{
    DrawCircle(p, size, color);
}

void DebugDraw::drawAabb(const b2AABB* aabb, const sf::Color& color)
{
    const auto polygon = std::make_shared<sf::ConvexShape>();
    polygon->setPointCount(4);
    polygon->setPoint(0, sf::Vector2(aabb->lowerBound.x * PhysicsManager::RATIO, aabb->lowerBound.y * PhysicsManager::RATIO));
    polygon->setPoint(1, sf::Vector2(aabb->upperBound.x * PhysicsManager::RATIO, aabb->lowerBound.y * PhysicsManager::RATIO));
    polygon->setPoint(2, sf::Vector2(aabb->upperBound.x * PhysicsManager::RATIO, aabb->upperBound.y * PhysicsManager::RATIO));
    polygon->setPoint(3, sf::Vector2(aabb->lowerBound.x * PhysicsManager::RATIO, aabb->upperBound.y * PhysicsManager::RATIO));

    polygon->setOutlineThickness(1.0F);
    polygon->setOutlineColor(color);
    polygon->setFillColor(sf::Color::Transparent);
    m_shapes.push_back(polygon);
}

void DebugDraw::drawText(const std::string& str, const sf::Vector2f& center, const sf::Color& color)
{
    auto text = std::make_shared<sf::Text>();
    text->setPosition(center);
    text->setString(str);
    text->setFillColor(color);
    text->setFont(m_font);
    text->setCharacterSize(14);
    const auto bounds = text->getLocalBounds();
    text->setOrigin(bounds.width * 0.5F, bounds.height * 0.5F + bounds.top);

    m_shapes.push_back(text);
}

void DebugDraw::update(float deltaTime)
{
    if (InputManager::getInstance().isKeyReleased("debugdraw"))
    {
        setEnabled(!isEnabled());
    }
}

void DebugDraw::draw(sf::RenderWindow& renderWindow)
{
    if (m_enabled)
    {
        for (const auto& shape : m_shapes)
        {
            renderWindow.draw(*shape);
        }
    }

    m_shapes.clear();
}
} // namespace mmt_gd
