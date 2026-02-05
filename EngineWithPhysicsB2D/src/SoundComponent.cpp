#include "stdafx.h"

#include "GameObject.hpp"
#include "SoundComponent.hpp"
#include <random>

namespace mmt_gd
{

void SoundComponent::update(float deltaTime)
{
    sf::Vector2f pos = m_gameObject.getPosition();
    for (auto& [name, sound] : m_sounds)
    {
        if (sound.getStatus() == sf::Sound::Playing)
        {
            sound.setPosition(pos.x, pos.y, 0.f);
        }
    }
}

void SoundComponent::addSound(const std::string& soundName, const std::string& filePath)
{
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filePath))
    {
        m_buffers[soundName] = buffer;
        m_sounds[soundName].setBuffer(m_buffers[soundName]);
        m_sounds[soundName].setRelativeToListener(false); 
    }
}

void SoundComponent::addMusic(const std::string& musicName, const std::string& filePath)
{
    auto music = std::make_unique<sf::Music>();
    if (music->openFromFile(filePath))
    {
        m_musics[musicName] = std::move(music);
    }
}

void SoundComponent::playSound(const std::string& soundName)
{
    auto it = m_sounds.find(soundName);
    if (it != m_sounds.end())
    {
        // Random Pitch
        static std::mt19937                   gen(std::random_device{}());
        std::uniform_real_distribution<float> dist(m_minPitch, m_maxPitch);

        it->second.setPitch(dist(gen));

        // Positional Sound
        sf::Vector2f pos = m_gameObject.getPosition();
        it->second.setPosition(pos.x, pos.y, 0.f);
        it->second.setMinDistance(m_soundDistance);
        it->second.setAttenuation(1.0f);

        it->second.play();
    }
}

void SoundComponent::playMusic(const std::string& musicName, bool loop)
{
    if (m_musics.count(musicName))
    {
        m_musics[musicName]->setLoop(loop);
        m_musics[musicName]->play();
    }
}

void SoundComponent::stopMusic(const std::string& musicName)
{
    m_musics[musicName]->stop();
}

} // namespace mmt_gd