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
    ~SoundComponent() = default;

    void playSound(const std::string& soundName);
    void playMusic(const std::string& musicName, bool loop);
    void stopMusic(const std::string& musicName);
    void addSound(const std::string& soundName, const std::string& filePath);
    void addMusic(const std::string& musicName, const std::string& filePath);

    void update(float deltaTime) override;

private:
    std::unordered_map<std::string, sf::SoundBuffer>            m_buffers;
    std::unordered_map<std::string, sf::Sound>                  m_sounds;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_musics;

    const float m_defaultVolume = 100.0f;
    const float m_minPitch      = 0.9f;
    const float m_maxPitch      = 1.1f;
    const float m_soundDistance = 300.f;
};
} // namespace mmt_gd
