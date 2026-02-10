//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
#include "stdafx.h"

#include "InputManager.hpp"

namespace mmt_gd
{
InputManager& InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::process(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
    {
        bool isDown = (event.type == sf::Event::KeyPressed);
        if (event.key.code >= 0 && event.key.code < sf::Keyboard::KeyCount)
        {
            // Keyboard-Input steuert alle Player (Globales Device)
            for (int i = 0; i < PlayerCount; ++i)
            {
                m_eventFrame.m_keys[i][event.key.code] = isDown;
            }
        }
    }
    else if (event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased)
    {
        bool isDown = (event.type == sf::Event::JoystickButtonPressed);
        int playerIdx = event.joystickButton.joystickId;

        if (playerIdx >= 0 && playerIdx < PlayerCount)
        {
            if (event.joystickButton.button < sf::Joystick::ButtonCount)
                m_eventFrame.m_buttons[playerIdx][event.joystickButton.button] = isDown;
        }
    }
    else if (event.type == sf::Event::JoystickMoved)
    {
        int playerIdx = event.joystickMove.joystickId;

        if (playerIdx >= 0 && playerIdx < PlayerCount && event.joystickMove.axis < sf::Joystick::AxisCount)
        {
            m_eventFrame.m_axes[playerIdx][event.joystickMove.axis] = event.joystickMove.position;
        }
    }
}

void InputManager::update()
{
    m_lastFrame    = m_currentFrame;
    m_currentFrame = m_eventFrame;
}

void InputManager::bind(const std::string& action, sf::Keyboard::Key keyCode, int playerIdx)
{
    if (playerIdx < 0 || playerIdx >= 5)
        return;

    m_actionBinding[playerIdx][action] = {static_cast<int>(keyCode), InputDevice::Keyboard};
}

void InputManager::bind(const std::string& action, unsigned int buttonIdx, int playerIdx)
{
    if (playerIdx < 0 || playerIdx >= 5)
        return;

    m_actionBinding[playerIdx][action] = {static_cast<int>(buttonIdx), InputDevice::JoystickButton};
}

void InputManager::bind(const std::string& action, JoystickAxisData joystickData, int playerIdx)
{
    if (playerIdx < 0 || playerIdx >= 5)
        return;

    Binding binding;
    binding.type             = InputDevice::JoystickAxis;
    binding.joystickAxisData = joystickData;
    binding.code             = static_cast<int>(joystickData.axis);

    m_actionBinding[playerIdx][action] = binding;
}

void InputManager::unbind(const std::string& action, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_actionBinding[playerIdx].erase(action);
}

bool InputManager::getRawState(const std::string& action, int playerIdx, const FrameData& frame) const
{
    int code = getKeyForAction(action, playerIdx);
    if (code == -1)
        return false;

    const auto& binding = m_actionBinding[playerIdx].at(action);
    float       pos;

    switch (binding.type)
    {
        case InputDevice::Keyboard:
            return frame.m_keys[playerIdx][code];
        case InputDevice::JoystickButton:
            return frame.m_buttons[playerIdx][code];
        case InputDevice::JoystickAxis:
            pos = frame.m_axes[playerIdx][binding.joystickAxisData.axis];
            return (binding.joystickAxisData.isPositive) ? pos > binding.joystickAxisData.threshold
                                                         : pos < -binding.joystickAxisData.threshold;
        default:
            return false;
    }
}

int InputManager::getKeyForAction(const std::string& action, int playerIdx) const
{
    if (playerIdx < 0 || playerIdx >= PlayerCount)
        return -1;

    auto it = m_actionBinding[playerIdx].find(action);
    if (it != m_actionBinding[playerIdx].end())
    {
        return it->second.code;
    }
    return -1;
}

bool InputManager::isKeyDown(const std::string& action, int playerIdx)
{
    return getRawState(action, playerIdx, m_currentFrame);
}

bool InputManager::isKeyUp(const std::string& action, int playerIdx)
{
    return !isKeyDown(action, playerIdx);
}

bool InputManager::isKeyPressed(const std::string& action, int playerIdx)
{
    return getRawState(action, playerIdx, m_currentFrame) && !getRawState(action, playerIdx, m_lastFrame);
}

bool InputManager::isKeyReleased(const std::string& action, int playerIdx)
{
    return !getRawState(action, playerIdx, m_currentFrame) && getRawState(action, playerIdx, m_lastFrame);
}

void InputManager::clear()
{
    for (int i = 0; i < PlayerCount; ++i)
    {
        m_actionBinding[i].clear();
    }
}

sf::Vector2f InputManager::getMousePosition() const
{
    ffAssertMsg(m_renderWindow != nullptr, "RenderWindow not set!");

    return m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow));
}
} // namespace mmt_gd
