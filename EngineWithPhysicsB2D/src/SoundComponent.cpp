#include "stdafx.h"

#include "GameObject.hpp"
#include "SoundComponent.hpp"
#include "SFML/Audio.hpp"
#include <random>

namespace mmt_gd
{

void SoundComponent::addSound(const std::string& soundName, std::string filePath, float volume = 100.0f)
{
    if (!m_soundsBuffers[soundName].loadFromFile(filePath))
    {
        sf::err() << "Could not load sound: " << filePath << std::endl;
        return; // Stop if loading fails
    }
    m_sounds[soundName].setBuffer(m_soundsBuffers[soundName]);
    m_sounds[soundName].setVolume(volume);
    m_sounds[soundName].setRelativeToListener(false);
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
} // namespace mmt_gd