#pragma once

#include "FPS.hpp"
#include "GameStateManager.hpp"

#include <string>

namespace mmt_gd
{
class InputManager;
class DebugDraw;

class Game
{
public:
    struct Config
    {
        sf::Vector2i m_resolution{800, 600};
        std::string  m_windowName = "FinalFrontier";
    };

    Config& getConfig()
    {
        return m_config;
    }

    sf::RenderWindow& getWindow()
    {
        return m_window;
    }

    tgui::Gui& getGui()
    {
        return m_gui;
    }

    void run();

private:
    bool init();
    void initInputManager();
    void update();
    void draw();
    void shutdown() const;

    Config m_config;

    sf::RenderWindow m_window;
    GameStateManager m_gameStateManager;

    InputManager* m_inputManager = nullptr;
    DebugDraw*    m_debugDraw    = nullptr;
    Fps           m_fps;
    tgui::Gui     m_gui;
};
} // namespace mmt_gd
