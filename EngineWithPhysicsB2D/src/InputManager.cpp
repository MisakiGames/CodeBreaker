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
            // Keyboard-Input wird für alle Player gespiegelt (Globales Device)
            for (int i = 0; i < PlayerCount; ++i)
            {
                m_eventFrame.m_keys[i][event.key.code] = isDown;
            }
        }
    }
    else if (event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased)
    {
        int playerIdx = event.joystickButton.joystickId + 1;
        if (playerIdx >= 5 || event.joystickButton.button >= sf::Joystick::ButtonCount)
            return;

        bool isDown = (event.type == sf::Event::JoystickButtonPressed);
        m_eventFrame.m_buttons[playerIdx][event.joystickButton.button] = isDown;
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

    m_actionBinding[playerIdx][action] = {static_cast<int>(buttonIdx), InputDevice::Joystick};
}

void InputManager::unbind(const std::string& action, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_actionBinding[playerIdx].erase(action);
}

int InputManager::getKeyForAction(const std::string& action, int playerIdx)
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
    int code = getKeyForAction(action, playerIdx);
    if (code == -1)
        return false;

    const auto& binding = m_actionBinding[playerIdx].at(action);
    if (binding.type == InputDevice::Keyboard)
    {
        return m_currentFrame.m_keys[playerIdx][code];
    }
    else
    {
        return m_currentFrame.m_buttons[playerIdx][code];
    }
}

bool InputManager::isKeyUp(const std::string& action, int playerIdx)
{
    int code = getKeyForAction(action, playerIdx);
    if (code == -1)
        return true;

    const auto& binding = m_actionBinding[playerIdx].at(action);
    if (binding.type == InputDevice::Keyboard)
    {
        return !m_currentFrame.m_keys[playerIdx][code];
    }
    else
    {
        return !m_currentFrame.m_buttons[playerIdx][code];
    }
}

bool InputManager::isKeyPressed(const std::string& action, int playerIdx)
{
    int code = getKeyForAction(action, playerIdx);
    if (code == -1)
        return false;

    const auto& binding = m_actionBinding[playerIdx].at(action);
    if (binding.type == InputDevice::Keyboard)
    {
        return m_currentFrame.m_keys[playerIdx][code] && !m_lastFrame.m_keys[playerIdx][code];
    }
    else
    {
        return m_currentFrame.m_buttons[playerIdx][code] && !m_lastFrame.m_buttons[playerIdx][code];
    }
}

bool InputManager::isKeyReleased(const std::string& action, int playerIdx)
{
    int code = getKeyForAction(action, playerIdx);
    if (code == -1)
        return false;

    const auto& binding = m_actionBinding[playerIdx].at(action);
    if (binding.type == InputDevice::Keyboard)
    {
        return !m_currentFrame.m_keys[playerIdx][code] && m_lastFrame.m_keys[playerIdx][code];
    }
    else
    {
        return !m_currentFrame.m_buttons[playerIdx][code] && m_lastFrame.m_buttons[playerIdx][code];
    }
}

void InputManager::init()
{
    bind("Exit", sf::Keyboard::Escape, 0);
    bind("debugdraw", sf::Keyboard::F1, 0);

    bind("up", sf::Keyboard::Up, 0);
    bind("left", sf::Keyboard::Left, 0);
    bind("down", sf::Keyboard::Down, 0);
    bind("right", sf::Keyboard::Right, 0);
}

void InputManager::shutdown()
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
