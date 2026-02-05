#pragma once

#include "IComponent.hpp"

#include <SFML/Audio.hpp>
#include <unordered_map>

namespace mmt_gd
{

class SoundComponent : public IComponent
{
public:
    void playSound(const std::string& soundName);
    void playMusic(const std::string& musicName);
    void addSound(const std::string& soundName, const std::string filePath);
    void addMusic(const std::string& musicName, const std::string filePath);

private:
    std::unordered_map<std::string, sf::Sound> m_sounds;
    std::unordered_map<std::string, sf::Music> m_musics;
};
} // namespace mmt_gd
