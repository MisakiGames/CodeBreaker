//This code was made for the Multimedia Project 2a for the Multimedia Technology class in the Fachhochschule Salzburg, by Christopher Kastner and Tim Paul
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

    void playSound(const std::string& soundName, bool checkIfPlay = false);


    void addSound(const std::string& soundName, std::string filePath, float volume);


private:
    std::unordered_map<std::string, sf::Sound>                  m_sounds;
    std::unordered_map<std::string, sf::SoundBuffer>                  m_soundsBuffers;


    const float m_minPitch      = 0.9f;
    const float m_maxPitch      = 1.1f;
    const std::string m_defaultSoundPath = "../assets/sounds/";
};
} // namespace mmt_gd
