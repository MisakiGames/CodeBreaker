#pragma once

#include "IComponent.hpp"

#include <SFML/Audio.hpp>
#include <unordered_map>

namespace mmt_gd
{

class SoundComponent : public IComponent
{
public:

public:
    explicit SoundComponent(GameObject& gameObject) : IComponent(gameObject)
    {
    }
    ~SoundComponent() override = default;

    bool init() override
    {
        return true;
    }
    void update(float deltaTime)
    {
        return;
    }

    void playSound(const std::string& soundName);
    void playMusic(const std::string& musicName, bool loop = true);
    void stopMusic(const std::string& musicName);

    void addSound(const std::string& soundName, std::string filePath, float volume);
    void addMusic(const std::string& musicName, const std::string& filePath, float volume);

private:
    std::unordered_map<std::string, sf::Sound>                  m_sounds;
    std::unordered_map<std::string, sf::SoundBuffer>                  m_soundsBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_musics;

    const float m_minPitch      = 0.9f;
    const float m_maxPitch      = 1.1f;
    const std::string m_defaultSoundPath = "../assets/sounds/";
};
} // namespace mmt_gd
