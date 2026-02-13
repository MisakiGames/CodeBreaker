//This code was made for the Multimedia Project 2a,
//in the Multimedia Technology class at the FH Salzburg,
//by Christopher Kastner and Tim Paul
#include "stdafx.hpp"

#include "MusicComponent.hpp"

#include "GameObject.hpp"
#include "SFML/Audio.hpp"

#include <random>

namespace mmt_gd
{
void MusicComponent::addMusic(const std::string& musicName, const std::string& filePath, float volume)
{
    auto music = std::unique_ptr<sf::Music>(new sf::Music());

    if (music->openFromFile(filePath))
    {
        music->setVolume(volume);
        m_musics.emplace(musicName, std::move(music));
    }
    else
    {
#ifdef DEBUG
        sf::err() << "Could not load music: " << filePath << std::endl;
#endif
    }
}

void MusicComponent::playMusic(const std::string& musicName, bool loop)
{
    if (m_musics.find(musicName) != m_musics.end())
    {
        m_musics[musicName]->setLoop(loop);
        m_musics[musicName]->play();
    }
}
void MusicComponent::stopMusic(const std::string& musicName)
{
    if (m_musics.find(musicName) != m_musics.end())
    {
        m_musics[musicName]->stop();
    }
}
} // namespace mmt_gd