#include "stdafx.h"

#include "GameObject.hpp"
#include "SoundComponent.hpp"
#include "SFML/Audio.hpp"
#include <random>

namespace mmt_gd
{

void SoundComponent::addSound(const std::string& soundName, std::string filePath, float volume = 100.0f)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath))
    {
        sf::err() << "Could not load sound: " << filePath<< std::endl;
    }
    m_soundsBuffers.emplace(soundName, buffer);
    m_sounds[soundName].setBuffer(buffer);
    m_sounds[soundName].setVolume(volume);
    m_sounds[soundName].setRelativeToListener(false);
}

void SoundComponent::addMusic(const std::string& musicName, const std::string& filePath, float volume)
{
    auto        music    = std::make_unique<sf::Music>();
    std::string fullPath = m_defaultSoundPath + filePath;

    if (music->openFromFile(fullPath))
    {
        music->setVolume(volume); // Lautstärke für diesen Musik-Track setzen
        m_musics[musicName] = std::move(music);
    }
    else
    {
        sf::err() << "Could not load music: " << fullPath << std::endl;
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