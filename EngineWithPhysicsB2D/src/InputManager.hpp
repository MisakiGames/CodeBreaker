#pragma once

#include "SFML/Window.hpp"

#include <unordered_map>

namespace mmt_gd
{

enum class InputDevice
{
    Keyboard,
    JoystickButton,
    JoystickAxis
};

class InputManager
{
public:
    static InputManager& getInstance();
    InputManager(const InputManager& rhv)             = delete;
    InputManager(InputManager&& rhv)                  = delete;
    InputManager&  operator=(const InputManager& rhv) = delete;
    InputManager&& operator=(InputManager&& rhv)      = delete;

    struct JoystickAxisData
    {
        sf::Joystick::Axis axis;
        float              threshold;
        bool               isPositive; // true für nach rechts/unten, false für links/oben
    };

    /**
     * \brief Must be called at beginning of game update, before the update
    ´* method. The events are
     * classified and processed internally.
     * \param event a single event coming from the sf polling.
     */
    void process(const sf::Event& event);

    /**
     * \brief Must be called after all events have been processed.
     */
    void update();

    /**
     * \brief Binds an action to the given keycode and for the given player
    ´* \param action the name of the action (eg. left, jump, ...)
     * \param keyCode the keycode of the action
     * \param playerIdx the idx of the player
     */
    void bind(const std::string& action, sf::Keyboard::Key keyCode, int playerIdx);
    void bind(const std::string& action, unsigned int buttonIdx, int playerIdx);
    void bind(const std::string& action, JoystickAxisData joystickData, int playerIdx);

    void unbind(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action is currently down.
     */
    bool isKeyDown(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action is currently up.
     */
    bool isKeyUp(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action has been pressed.
     */
    bool isKeyPressed(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action has been released.
     */
    bool isKeyReleased(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns the current mouse position relative to the window
     */
    [[nodiscard]] sf::Vector2f getMousePosition() const;

    void setRenderWindow(sf::RenderWindow* window)
    {
        m_renderWindow = window;
    }

    void clear();
    int  getPlayerCount()
    {
        return PlayerCount;
    };

private:
    InputManager()  = default;
    ~InputManager() = default;

    static constexpr int PlayerCount = 4; ///< maximum allowed players. Can be increased if needed.

    struct FrameData
    {
        bool  m_keys[PlayerCount][sf::Keyboard::KeyCount]       = {{false}};
        bool  m_buttons[PlayerCount][sf::Joystick::ButtonCount] = {{false}};
        float m_axes[PlayerCount][sf::Joystick::AxisCount]      = {{0.0f}}; 
    };

    struct Binding
    {
        int          code;
        InputDevice  type;
        JoystickAxisData joystickAxisData;
    };

    bool getRawState(const std::string& action, int playerIdx, const FrameData& frame) const;
    int  getKeyForAction(const std::string& action, int playerIdx) const;

    FrameData m_lastFrame{};
    FrameData m_currentFrame{};
    FrameData m_eventFrame{};

    sf::RenderWindow*                        m_renderWindow{nullptr};
    std::unordered_map<std::string, Binding> m_actionBinding[PlayerCount];
};
} // namespace mmt_gd
