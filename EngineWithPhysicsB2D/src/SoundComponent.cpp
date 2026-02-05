#include "stdafx.h"

#include "SoundComponent.hpp"

namespace mmt_gd
{

void SoundComponent::playSound(const std::string& soundName)
{
    auto it = m_sounds.find(soundName);
    if (it != m_sounds.end())
    {
        it->second.play();
    }
}

void SoundComponent::addSound(const std::string& soundName, const std::string filePath)
{
    sf::Sound       sound;
    sf::SoundBuffer buffer;

    if (buffer.loadFromFile(filePath))
    {
        sound.setBuffer(buffer);
        m_sounds[soundName] = sound;
    }
}


} // namespace mmt_gd